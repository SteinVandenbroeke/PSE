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
    fvaccins = 0;
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
    return this->fvaccins;
}

int VaccinationCenter::getVaccinated() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fvaccinated;
}

void VaccinationCenter::addVaccins(const int amount, const Vaccin* vaccin) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    fvaccins += amount;

    if (this->fvaccinsType.find(vaccin->getType()) == this->fvaccinsType.end()) {

        VaccinationCenter::vaccinType newVaccin(vaccin->getType(), vaccin->getTemperature(),
                                                vaccin->getRenewal(), 0);

        this->fvaccinsType.insert(std::make_pair(newVaccin.fvaccinType, newVaccin));
    }
    this->fvaccinsType.find(vaccin->getType())->second.fvaccinAmount += amount;

    ENSURE(fvaccins <= fcapacity * 2, "Amount of vaccins must not exceed capacity of Center");
}

int VaccinationCenter::calculateVaccinationAmount() {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int notVaccinated = fpopulation - fvaccinated;
    int smallest = std::min(fvaccins, fcapacity);
    return std::min(smallest, notVaccinated);
}

int VaccinationCenter::calculateVaccinationAmount(const VaccinationCenter::vaccinType & vaccin) const{

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int notVaccinated = fpopulation - fvaccinated;
    int smallest = std::min(vaccin.fvaccinAmount, fcapacity);
    return std::min(smallest, notVaccinated);
}

void VaccinationCenter::vaccinateCenter(std::ostream &stream) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(fvaccins <= fcapacity * 2, "Amount of vaccins must not exceed capacity");

//    int vaccinationAmount = calculateVaccinationAmount();
//
//    fvaccins -= vaccinationAmount;
//    fvaccinated += vaccinationAmount;
//

    int vaccinationAmount = 0;
    for (std::map<const std::string, VaccinationCenter::vaccinType>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {

        vaccinationAmount += calculateVaccinationAmount(it->second);
        it->second.fvaccinAmount -= calculateVaccinationAmount(it->second);
    }
    fvaccins -= vaccinationAmount;
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
    int perVaccin = ToPercent(fvaccins, fcapacity);
    int perVaccinated = ToPercent(fvaccinated, fpopulation);

    stream << this->fname << ":" << "\n";
    stream << "\t" << "- " << "vaccins       " << ProgressBar(perVaccin, 20)   << " "<< perVaccin << "%" <<"\n";
    stream << "\t" << "- " << "geavaccineerd " << ProgressBar(perVaccinated, 20) << " " <<  perVaccinated << "%" << "\n";
}

void VaccinationCenter::printVaccins(std::ostream &stream) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int perVaccin = ToPercent(fvaccins, fcapacity);
    int perVaccinated = ToPercent(fvaccinated, fpopulation);

    stream << this->fname << ":" << "\n";
    stream << "\t" << "- " << "vaccins       " << ProgressBar(perVaccin, 20)   << " "<< perVaccin << "%" <<"\n";
    stream << "\t" << "- " << "geavaccineerd " << ProgressBar(perVaccinated, 20) << " " <<  perVaccinated << "%" << "\n";

    for (std::map<const std::string, VaccinationCenter::vaccinType>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {

        stream << it->first << ":" << "\n";
        stream << "\t" << "- " << "vaccins       " << it->second.fvaccinAmount << "\n";
    }
}

VaccinationCenter::vaccinType::vaccinType(const std::string &vaccinType, int vaccinTemperature, int vaccinRenewal, int vaccinAmount)
        : fvaccinType(vaccinType), fvaccinTemperature(vaccinTemperature), fvaccinRenewal(vaccinRenewal), fvaccinAmount(vaccinAmount), ftracker(0) {}
