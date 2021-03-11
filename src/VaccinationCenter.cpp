/**
 * @file VaccinationCenter.cpp
 * @brief This file contains the definitions of the members of the VaccinationCenter class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#include "VaccinationCenter.h"

VaccinationCenter::VaccinationCenter() {

    _initCheck = this;
    fname = "";
    faddress = "";
    fpopulation = 0;
    fcapacity = 0;
    fvaccins = 0;
    fvaccinated = 0;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

VaccinationCenter::VaccinationCenter(const std::string &fname, const std::string &faddress, int fpopulation
                                     ,int fcapacity) :
    fname(fname), faddress(faddress), fpopulation(fpopulation),fcapacity(fcapacity){

    _initCheck = this;
    fvaccins = 0;
    fvaccinated = 0;
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

void VaccinationCenter::addVaccins(int amount) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    fvaccins += amount;
    ENSURE(fvaccins <= fcapacity * 2, "Amount of vaccins must not exceed capacity of Center");
}

int VaccinationCenter::calculateVaccinationAmount() {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int notVaccinated = fpopulation - fvaccinated;
    int smallest = std::min(fvaccins, fcapacity);
    return std::min(smallest, notVaccinated);
}

void VaccinationCenter::vaccinateCenter() {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(fvaccins <= fcapacity * 2, "Amount of vaccins must not exceed capacity");

    int vaccinationAmount = calculateVaccinationAmount();

    fvaccins -= vaccinationAmount;
    fvaccinated += vaccinationAmount;

    std::cout << "Er werden " << vaccinationAmount << " inwoners gevaccineerd in " << fname << "." << std::endl;
}


void VaccinationCenter::print(std::ofstream &stream) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    stream << this->fname << ": " << this->fvaccinated << " gevaccineerd, nog ";
    stream << (this->fpopulation - this->fvaccinated) << " inwoners niet gevaccineerd\n";
}






