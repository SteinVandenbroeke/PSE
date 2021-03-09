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
        _initCheck = this;
        ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool Hub::properlyInitialized() const {

    return Hub::_initCheck == this;
}

void Hub::addCenter(const std::string &name, VaccinationCenter *center) {

    this->fcentra[name] = center;
}

void Hub::updateCenter(const std::string &name, VaccinationCenter *center) {

    if(this->fcentra[name]){
        delete this->fcentra[name];
        this->fcentra[name] = center;
    }
}

void Hub::print(std::ofstream &stream) {

    stream << "Hub (" << fvaccin << " vaccins)\n";
    for(std::map<std::string,VaccinationCenter*>::iterator it = this->fcentra.begin(); it != this->fcentra.end(); it++){
        stream << " -> " << it->first << " (" << (it->second)->getVaccins() << " vaccins)\n";
    }
    stream << "\n";
}


