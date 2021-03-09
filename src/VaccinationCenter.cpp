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

VaccinationCenter::VaccinationCenter(const std::string &fname, const std::string &faddress, int fpopulation
                                     ,int fcapacity) :

    fname(fname), faddress(faddress), fpopulation(fpopulation),fcapacity(fcapacity){
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

    stream << this->fname << ": " << this->fvaccinated << " gevaccineerd, nog ";
    stream << (this->fpopulation - this->fvaccinated) << " inwoners niet gevaccineerd\n";
}




