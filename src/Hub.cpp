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

int Hub::getFdelivery() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return fdelivery;
}

int Hub::getFinterval() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return finterval;
}

int Hub::getFtransport() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return ftransport;
}

int Hub::getFvaccin() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return fvaccin;
}

const std::map<std::string, VaccinationCenter *> &Hub::getFcentra() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return fcentra;
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

void Hub::updateVaccins() {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    fvaccin += fdelivery;
}

int Hub::calculateTransport(const VaccinationCenter* center) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    int cargo = 0;
    int vaccinsTransport = 0;

    while ((((ftransport * cargo) + center->getVaccins()) <= (2 * center->getCapacity())) &&
           (ftransport * cargo) <= fvaccin) {

        if ((ftransport * cargo) + center->getVaccins() > center->getCapacity()) {

            if (((ftransport * cargo) + center->getVaccins()) - center->getCapacity() < ftransport) {
                vaccinsTransport = cargo * ftransport;
                cargo ++;
                break;
            }
            else {
                break;
            }
        }
        vaccinsTransport = cargo * ftransport;
        cargo ++;
    }
    return vaccinsTransport;
}


void Hub::transportVaccin(const std::string &centerName) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    VaccinationCenter* center = fcentra.find(centerName)->second;
    REQUIRE(center->getName() == centerName, "Name of found center and given center name must be equal");

    int vaccinsHub = fvaccin;
    int vaccinsCenter = center->getVaccins();

    // Calculate cargo
    int vaccinsTransport = calculateTransport(center);
    int cargo = vaccinsTransport / ftransport;

    // Display nothing
    if (cargo == 0) {
        return;
    }

    // Update amount of vaccins in Hub and Center
    fvaccin -= vaccinsTransport;
    center->addVaccins(vaccinsTransport);

    // Display information of transport
    std::cout << "Er werden " << cargo << " ladingen (" << vaccinsTransport << " vaccins) getransporteerd naar ";
    std::cout << center->getName() << "." << std::endl;

    ENSURE(vaccinsHub != fvaccin, "Amount of vaccins in Hub must be updated");
    ENSURE(vaccinsCenter != center->getVaccins(), "Amount of vaccins in VaccinationCenter must be updated");
}

void Hub::print(std::ofstream &stream) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    stream << "Hub (" << fvaccin << " vaccins)\n";

    // Traverse VaccinationCentra
    for(std::map<std::string,VaccinationCenter*>::const_iterator it = this->fcentra.begin(); it != this->fcentra.end(); it++){
        REQUIRE(it->second->properlyInitialized(), "VaccinationCenter must be properly initialized");
        stream << " -> " << it->first << " (" << (it->second)->getVaccins() << " vaccins)\n";
    }
    stream << "\n";
}
