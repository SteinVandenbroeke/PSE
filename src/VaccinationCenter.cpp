/**
 * @file VaccinationCenter.cpp
 * @brief This file contains the definitions of the members of the VaccinationCenter class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#include "VaccinationCenter.h"
#include "Vaccin.h"

VaccinationCenter::VaccinationCenter(const std::string &fname, const std::string &faddress, int fpopulation
                                     ,int fcapacity) :
    fname(fname), faddress(faddress), fpopulation(fpopulation),fcapacity(fcapacity){
    REQUIRE(fname.length() > 0, "Name can't be empty");
    REQUIRE(fname.length() > 0, "Adres can't be empty");
    REQUIRE(fpopulation >= 0, "Negative population'");
    REQUIRE(fcapacity >= 0, "Negative capacity");

    _initCheck = this;
//    fvaccins = 0;
    fvaccinated = 0;
    fvaccinsType.clear();
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool VaccinationCenter::properlyInitialized() const {

    return VaccinationCenter::_initCheck == this;
}

const std::string &VaccinationCenter::getName() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fname;
}

const std::string &VaccinationCenter::getAddress() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->faddress;
}

int VaccinationCenter::getPopulation() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fpopulation;
}

int VaccinationCenter::getCapacity() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fcapacity;
}

int VaccinationCenter::getVaccins() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    int totalVaccins = 0;
    for(std::map<const std::string, vaccinType>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
        totalVaccins += it->second.;
    }
    return totalVaccins;
}

int VaccinationCenter::getVaccinated() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fvaccinated;
}

void VaccinationCenter::addVaccins(const int amount, const Vaccin* vaccin) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    if (this->fvaccinsType.find(vaccin->getType()) == this->fvaccinsType.end()) {

        VaccinationCenter::vaccinType newVaccin(vaccin->getType(), vaccin->getTemperature(),
                                                vaccin->getRenewal(), 0);

        this->fvaccinsType.insert(std::make_pair(newVaccin.fvaccinType, newVaccin));
    }
    this->fvaccinsType.find(vaccin->getType())->second.fvaccinAmount += amount;

    ENSURE(this->getVaccins() <= fcapacity * 2, "Amount of vaccins must not exceed capacity of Center");
}

int VaccinationCenter::calculateVaccinationAmount() {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int notVaccinated = fpopulation - fvaccinated;
    int smallest = std::min(this->getVaccins(), fcapacity);
    return std::min(smallest, notVaccinated);
}

int VaccinationCenter::calculateVaccinationAmount(const VaccinationCenter::vaccinType & vaccin) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int notVaccinated = fpopulation - fvaccinated;
    int smallest = std::min(vaccin.fvaccinAmount, fcapacity);
    return std::min(smallest, notVaccinated);
}

int VaccinationCenter::calculateVaccinationAmountRenewal(VaccinationCenter::vaccinType & vaccin, const int vaccinated) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");


    int smallest = std::min(vaccin.fvaccinAmount, fcapacity);
    int secondShot = std::min(smallest, vaccinated);

    // No vaccins left over after vaccinating the already vaccinated population
    if (secondShot >= vaccin.fvaccinAmount) {

        return secondShot;
    }
    // Vaccins left over after vaccinating the already vaccinated population --> Distribute over population
    else {

        int notVaccinated = fpopulation - (fvaccinated + secondShot);
        int smallest_ = std::min(vaccin.fvaccinAmount - secondShot, fcapacity - secondShot);
        int firstShot = std::min(smallest_, notVaccinated);

        if (firstShot != 0) {

            vaccin.ftracker.push_back(std::make_pair(vaccin.fvaccinRenewal * (-1), firstShot));
        }

        // Substract first shot - secondShot will be substracted from callee side
        vaccin.fvaccinAmount -= firstShot;

        return secondShot;
    }
}

void VaccinationCenter::updateRenewal() {

    for (std::map<const std::string, VaccinationCenter::vaccinType>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {

        if (it->second.isRenewal() && !it->second.ftracker.empty()) {

            for (std::vector<std::pair<int, int> >::iterator ite = it->second.ftracker.begin();
                 ite != it->second.ftracker.end(); ite++) {

                if (ite->first < 0 && ite->first != 0) {
                    ite->first += 1;
                }
            }
        }
    }
}

void VaccinationCenter::vaccinateCenter(std::ostream &stream) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(this->getVaccins() <= fcapacity * 2, "Amount of vaccins must not exceed capacity");

    int vaccinationAmount = 0;
    for (std::map<const std::string, VaccinationCenter::vaccinType>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {

        // Vaccin with renewal
        if (it->second.isRenewal()) {

            // Population did not yet get a first Vaccin
            if (it->second.ftracker.empty()) {

                int amountVaccinated = calculateVaccinationAmount(it->second);
                it->second.fvaccinAmount -= amountVaccinated; // Substract from type vaccin
                it->second.ftracker.push_back(std::make_pair(it->second.fvaccinRenewal * (-1), amountVaccinated));
            }

            // Population already got a first Vaccin
            else {

                std::vector<std::pair<int, int> >&alreadyVaccinated = it->second.ftracker;

                for (std::vector<std::pair<int, int> >::iterator ite = alreadyVaccinated.begin();
                        ite != alreadyVaccinated.end(); ite++) {

                    // Renewal interval is over
                    if (ite->first == 0) {

                        int amountVaccinated = calculateVaccinationAmountRenewal(it->second, ite->second);
                        vaccinationAmount += amountVaccinated;
                        it->second.fvaccinAmount -= amountVaccinated;


                        // Check
                        ite->second -= amountVaccinated;

                    }
                }

                it->second.ftracker.erase(std::remove_if(it->second.ftracker.begin(), it->second.ftracker.end(),
                                                         check), it->second.ftracker.end());


            }
            continue;
        }

        // Vaccin without renewal
        vaccinationAmount += calculateVaccinationAmount(it->second);
        it->second.fvaccinAmount -= calculateVaccinationAmount(it->second);
    }

    fvaccinated += vaccinationAmount;

    stream << "Er werden " << vaccinationAmount << " inwoners gevaccineerd in " << fname << ".\n";
}

void VaccinationCenter::print(std::ofstream &stream) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    stream << this->fname << ": " << this->fvaccinated << " gevaccineerd, nog ";
    stream << (this->fpopulation - this->fvaccinated) << " inwoners niet gevaccineerd\n";
}

void VaccinationCenter::printGraphical(std::ostream &stream) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    // TODO - Geeft 200 % # vaccins wanneer = 2 * capacity?
    int perVaccin = ToPercent(this->getVaccins(), fcapacity);
    int perVaccinated = ToPercent(fvaccinated, fpopulation);

    if(perVaccin > 100) perVaccin = 100;

    stream << this->fname << ":" << "\n";
    stream << "\t" << "- " << "vaccins       " << ProgressBar(perVaccin, 20)   << " "<< perVaccin << "%" <<"\n";
    stream << "\t" << "- " << "geavaccineerd " << ProgressBar(perVaccinated, 20) << " " <<  perVaccinated << "%" << "\n";
}

void VaccinationCenter::printVaccins(std::ostream &stream) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int perVaccin = ToPercent(this->getVaccins(), fcapacity);
    int perVaccinated = ToPercent(fvaccinated, fpopulation);

    if(perVaccin > 100) perVaccin = 100;

    stream << this->fname << ":" << "\n";
    stream << "\t" << "- " << "vaccins       " << ProgressBar(perVaccin, 20)   << " "<< perVaccin << "%" <<"\n";
    stream << "\t" << "- " << "geavaccineerd " << ProgressBar(perVaccinated, 20) << " " <<  perVaccinated << "%" << "\n";

    for (std::map<const std::string, VaccinationCenter::vaccinType>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {

        stream << it->first << ":" << "\n";
        stream << "\t" << "- " << "vaccins       " << it->second.fvaccinAmount << "\n";
    }
}

std::map<std::string, int> VaccinationCenter::requiredAmountVaccinType() {
    std::map<std::string, int> requiredVaccin;
    for(std::map<const std::string, VaccinationCenter::vaccinType>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
        requiredVaccin[it->first] = it->second.ftracker[0];
    }
    return std::map<std::string, int>();
}


VaccinationCenter::vaccinType::vaccinType(const std::string &vaccinType, int vaccinTemperature, int vaccinRenewal, int vaccinAmount)
        : fvaccinType(vaccinType), fvaccinTemperature(vaccinTemperature), fvaccinRenewal(vaccinRenewal), fvaccinAmount(vaccinAmount) {}
