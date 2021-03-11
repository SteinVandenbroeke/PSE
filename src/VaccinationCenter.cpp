/**
 * @file VaccinationCenter.cpp
 * @brief This file contains the definitions of the members of the VaccinationCenter class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#include "VaccinationCenter.h"

VaccinationCenter::VaccinationCenter() {

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

VaccinationCenter::VaccinationCenter(const std::string &name, const std::string &address, int population
                                     ,int capacity) :

    fname(name), faddress(address), fpopulation(population),fcapacity(capacity){
    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool VaccinationCenter::properlyInitialized() const {

    return VaccinationCenter::_initCheck == this;
}

const std::string &VaccinationCenter::getName() const {
    return this->fname;
}

const std::string &VaccinationCenter::getAddress() const {
    return this->faddress;
}

int VaccinationCenter::getPopulation() const {

    return this->fpopulation;
}

int VaccinationCenter::getCapacity() const {
    return this->fcapacity;
}

int VaccinationCenter::getVaccins() const {

    return this->fvaccins;
}

int VaccinationCenter::getVaccinated() const {
    return this->fvaccinated;
}


void VaccinationCenter::print(std::ofstream &stream) {

    stream << getName() << ": " << getVaccinated() << " gevaccineerd, nog ";
    stream << (getPopulation() - getVaccinated()) << " inwoners niet gevaccineerd\n";
}

void VaccinationCenter::runVaccination(){
    int unvaccinatedPop = getPopulation() - getVaccinated();
    fvaccinated  += std::min(getVaccins(),getCapacity(), unvaccinatedPop);
}

int VaccinationCenter::getFreeStockSpace() const {
    return 2*getCapacity() - fvaccins;
}

bool VaccinationCenter::deliveryVaccines(int vaccinAmount) {
    if(vaccinAmount > getFreeStockSpace()){
        return false;
    }
    fvaccins += vaccinAmount;
    return true;
}




