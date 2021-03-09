/**
 * @file Hub.cpp
 * @brief This file contains the definitions of the members of the Hub class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#include "Hub.h"

Hub::Hub(int fdelivery, int finterval, int ftransport) :
        fdelivery(fdelivery), finterval(finterval),
        ftransport(ftransport) {

        // Initial delivery of vaccins
        fvaccin = fdelivery;
        _initCheck = this;
        ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool Hub::properlyInitialized() const {

    return Hub::_initCheck == this;
}

void Hub::addCenter(const std::string &name, VaccinationCenter *center) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(center->properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(fcentra.find(name) == fcentra.end() ,"VaccinationCenter must not exist yet in map");

    // Add center to map of Hub
    this->fcentra[name] = center;

    ENSURE( fcentra.find(name) != fcentra.end(),"VaccinationCenter must exist in map");
}

void Hub::updateCenter(const std::string &name, VaccinationCenter *center) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(center->properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(fcentra.find(name) != fcentra.end() ,"VaccinationCenter must exist in map");

    if(this->fcentra[name]){
        delete this->fcentra[name];
        this->fcentra[name] = center;
    }
    ENSURE( fcentra.find(name) != fcentra.end(),"VaccinationCenter must exist in map");
}

void Hub::print(std::ofstream &stream) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    stream << "Hub (" << fvaccin << " vaccins)\n";

    // Traverse VaccinationCentra
    for(std::map<std::string,VaccinationCenter*>::iterator it = this->fcentra.begin(); it != this->fcentra.end(); it++){
        ENSURE(it->second->properlyInitialized(), "VaccinationCenter must be properly initialized");
        stream << " -> " << it->first << " (" << (it->second)->getVaccins() << " vaccins)\n";
    }
    stream << "\n";
}


