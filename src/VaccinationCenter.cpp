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

    for (std::map<const std::string, vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {

        totalVaccins += it->second->getVaccinAmount();
    }
    return totalVaccins;
}

int VaccinationCenter::getVaccinated() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fvaccinated;
}

void VaccinationCenter::addVaccins(const int amount, const Vaccin* vaccin) {
    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(amount + this->getVaccins() <= (this->getCapacity() * 2), "Amount of vaccins must not exceed capacity of Center");

    if (this->fvaccinsType.find(vaccin->getType()) == this->fvaccinsType.end()) {

        VaccinationCenter::vaccinType* vaccinStruct = new VaccinationCenter::vaccinType(vaccin->getType(),
                                                                                       vaccin->getTemperature(),
                                                                                       vaccin->getRenewal(), 0);

        this->fvaccinsType.insert(std::make_pair(vaccinStruct->getVaccinType(), vaccinStruct));
    }
    this->fvaccinsType.find(vaccin->getType())->second->getVaccinAmount() += amount;

    ENSURE(this->getVaccins() <= fcapacity * 2, "Amount of vaccins must not exceed capacity of Center");
}

int VaccinationCenter::calculateVaccinationAmount() {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int notVaccinated = fpopulation - fvaccinated;
    int smallest = std::min(this->getVaccins(), fcapacity);
    return std::min(smallest, notVaccinated);
}

int VaccinationCenter::calculateVaccinationAmount(const VaccinationCenter::vaccinType* vaccin, int alreadyVaccinedToDay) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int notVaccinated = fpopulation - fvaccinated;
    int smallest = std::min(vaccin->getVaccinAmount(), fcapacity - alreadyVaccinedToDay);
    return std::min(smallest, notVaccinated);
}

int VaccinationCenter::calculateVaccinationAmountRenewal(VaccinationCenter::vaccinType* vaccin, const int vaccinated) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");


    int smallest = std::min(vaccin->getVaccinAmount(), fcapacity);
    int secondShot = std::min(smallest, vaccinated);

    // No vaccins left over after vaccinating the already vaccinated population
    if (secondShot >= vaccin->getVaccinAmount()) {

        return secondShot;
    }
    // Vaccins left over after vaccinating the already vaccinated population --> Distribute over population
    else {

        int notVaccinated = fpopulation - (fvaccinated + secondShot);
        int smallest_ = std::min(vaccin->getVaccinAmount() - secondShot, fcapacity - secondShot);
        int firstShot = std::min(smallest_, notVaccinated);

        if (firstShot != 0) {

            vaccin->insertRequerdDay(vaccin->getVaccinRenewal() * (-1), firstShot);
        }

        // Substract first shot - secondShot will be substracted from callee side
        vaccin->getVaccinAmount() -= firstShot;

        return secondShot;
    }
}

void VaccinationCenter::updateRenewal() {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    for (std::map<const std::string, VaccinationCenter::vaccinType*>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
            (it)->second->addDay();
    }
}

std::map<const std::string, VaccinationCenter::vaccinType*> VaccinationCenter::getVaccin(bool zeroVaccin) const {
    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    std::map<const std::string, vaccinType*> zeroVaccins;
    for (std::map<const std::string, VaccinationCenter::vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
        if((it->second->getVaccinTemperature() < 0 && zeroVaccin) || (it->second->getVaccinTemperature() > 0 && !zeroVaccin)) {
            zeroVaccins.insert(std::make_pair(it->first, it->second));
        }
    }
    return zeroVaccins;
}



int VaccinationCenter::vaccinateCenter(std::map<const std::string, vaccinType*> vaccinsType, std::ostream &stream, int alreadyVaccinedToDay = 0) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(this->getVaccins() <= fcapacity * 2, "Amount of vaccins must not exceed capacity");

    int vaccinationAmount = 0;
    for (std::map<const std::string, VaccinationCenter::vaccinType*>::iterator it = vaccinsType.begin(); it != vaccinsType.end(); it++) {

        // Vaccin with renewal
        if (it->second->isRenewal()) {

            // Population did not yet get a first Vaccin
            if (it->second->getTracker().empty()) {

                int amountVaccinated = calculateVaccinationAmount(it->second, alreadyVaccinedToDay);
                it->second->getVaccinAmount() -= amountVaccinated; // Substract from type vaccin

                it->second->insertRequerdDay(it->second->getVaccinRenewal() * (-1), amountVaccinated);
            }

            // Population already got a first Vaccin
            else {
                std::map<int, int> &alreadyVaccinated = it->second->getTracker();

                for (std::map<int, int>::iterator ite = alreadyVaccinated.begin(); ite != alreadyVaccinated.end(); ite++) {

                    // Renewal interval is over
                    if (ite->first == 0) {
                        int amountVaccinated = calculateVaccinationAmountRenewal(it->second, ite->second);
                        vaccinationAmount += amountVaccinated;
                        it->second->getVaccinAmount() -= amountVaccinated;

                        // Check
                        ite->second -= amountVaccinated;

                    }

                }
//                it->second.getTracker().erase(std::remove_if(it->second.getTracker().begin(), it->second.getTracker().end(),
//                                                         check), it->second.getTracker().end());
            }
        }
        // Vaccin without renewal
        else {
            vaccinationAmount += calculateVaccinationAmount(it->second, alreadyVaccinedToDay);
            it->second->getVaccinAmount() -= calculateVaccinationAmount(it->second, alreadyVaccinedToDay);
        }

    }
    fvaccinated += vaccinationAmount;

    return vaccinationAmount;
}

void VaccinationCenter::print(std::ostream &stream) const {

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
    stream << "\t" << "- " << "geavaccineerd " << ProgressBar(perVaccinated, 20) << " " <<  perVaccinated << "%" << "\n";
    stream << "\t \t- " << "Totaal volledig: " << ": " << fvaccinated << "/" << fpopulation << "\n";
    stream << "\t" << "- " << "vaccins       " << ProgressBar(perVaccin, 20)   << " "<< perVaccin << "%" <<"\n";
    for (std::map<const std::string, VaccinationCenter::vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {
        stream << "\t \t- " << it->first << ": " << it->second->getVaccinAmount() << "\n";
        stream << "\t \t \t- " << "eerste prik: " << ": " << it->second->totalFirstVaccination() << "\n";
    }
    stream << "---------------" << std::endl;
}

std::map<std::string, int> VaccinationCenter::requiredAmountVaccinType() {
    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    std::map<std::string, int> requiredVaccin;
    for(std::map<const std::string, VaccinationCenter::vaccinType*>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
        if(requiredAmountVaccin(it->second) != 0) {
            requiredVaccin[it->first] = requiredAmountVaccin(it->second);
        }
    }
    return requiredVaccin;
}



void VaccinationCenter::vaccinateCenter(std::ostream &stream) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int totaal = vaccinateCenter(getVaccin(true), stream);
    totaal += vaccinateCenter(getVaccin(false), stream, totaal);

    stream << "Er werden " << totaal << " inwoners gevaccineerd in " << this->fname << ".\n";
}

int VaccinationCenter::getOpenVaccinStorage(Vaccin* vaccin) {
    //return: hoeveel vaccins er nog kunnen worden opgeslagen
    if((this->getPopulation() - this->getVaccinated()) <= this->getVaccins()){
        //indien er voldoende vaccins op vooraad zijn om iedereen te vaccineren
        return 0;
    }
    std::map<const std::string, vaccinType*> zeroVaccins;
    int openVaccinStorageTotal = this->getCapacity() * 2;
    int openVaccinStorage = openVaccinStorageTotal;
    if(vaccin->ifUnderZero()){
        openVaccinStorage = this->getCapacity();
    }

    for (std::map<const std::string, VaccinationCenter::vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
        if((it->second->getVaccinTemperature() < 0 && vaccin->ifUnderZero()) || (it->second->getVaccinTemperature() > 0 && !vaccin->ifUnderZero())) {
            openVaccinStorage -= it->second->getVaccinAmount();
        }
        openVaccinStorageTotal -= it->second->getVaccinAmount();
    }

    int minimum = std::min(openVaccinStorageTotal, openVaccinStorage);
    return std::min(minimum, std::max(this->getPopulation() - this->getVaccinated() - this->getVaccins(), vaccin->getTransport()));
}

int VaccinationCenter::requiredAmountVaccin(VaccinationCenter::vaccinType *vaccin) {
    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    int needed = vaccin->getTracker()[0] - vaccin->getVaccinAmount();
    if(needed < 0)
        needed = 0;

    return needed;
}

VaccinationCenter::vaccinType::vaccinType(const std::string &vaccinType, int vaccinTemperature, int vaccinRenewal, int vaccinAmount)
        : fvaccinType(vaccinType), fvaccinTemperature(vaccinTemperature), fvaccinRenewal(vaccinRenewal), fvaccinAmount(vaccinAmount) {}
