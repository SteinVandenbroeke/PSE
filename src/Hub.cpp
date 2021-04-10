/**
 * @file Hub.cpp
 * @brief This file contains the definitions of the members of the Hub class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#include "Hub.h"

Hub::Hub(){
    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool Hub::properlyInitialized() const {

    return Hub::_initCheck == this;
}

int Hub::getDelivery() const {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalDelivery = 0;
    for(std::vector<Vaccin *>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalDelivery += (*it)->getDelivery();
    }
    return totalDelivery;
}

int Hub::getInterval() const {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalInterval = 0;
    for(std::vector<Vaccin *>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalInterval += (*it)->getInterval();
    }
    return totalInterval;
}

int Hub::getTransport() const {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalTransport = 0;
    for(std::vector<Vaccin *>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalTransport += (*it)->getTransport();
    }
    return totalTransport;
}

int Hub::getVaccin() const {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalVaccins = 0;
    for(std::vector<Vaccin *>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalVaccins += (*it)->getVaccin();
    }
    return totalVaccins;
}

const std::map<std::string, VaccinationCenter *> &Hub::getCentra() const {

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

void Hub::updateVaccins() {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    for(std::vector<Vaccin *>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        (*it)->updateVaccins();
    }
}

int Hub::calculateTransport(const VaccinationCenter* center) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    //TODO
    int cargo = 0;
    int vaccinsTransport = 0;

    if (ftransport == 0) {
        return vaccinsTransport;
    }

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

    ENSURE(vaccinsTransport <= fvaccin, "Amount of vaccinsTransport is too high");
    ENSURE((vaccinsTransport <= (2 * center->getCapacity())), "Amount of vaccinsTransport is too high");
    return vaccinsTransport;
}


void Hub::transportVaccin(const std::string &centerName, std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(fcentra.find(centerName) != fcentra.end(), "Given centerName must exist");
    VaccinationCenter* center = fcentra[centerName];
    REQUIRE(center->getName() == centerName, "Name of found center and given center name must be equal");

    int vaccinsHub = getVaccin();
    int vaccinsCenter = center->getVaccins();

    // Calculate cargo
    int vaccinsTransport = calculateTransport(center);
    int cargo = vaccinsTransport / getTransport();

    // Display nothing
    if (cargo == 0) {
        return;
    }

    // Update amount of vaccins in Hub and Center
    fvaccin -= vaccinsTransport;
    center->addVaccins(vaccinsTransport);

    // Display information of transport
    stream << "Er werden " << cargo << " ladingen (" << vaccinsTransport << " vaccins) getransporteerd naar ";
    stream << center->getName() << ".\n";

    ENSURE(vaccinsHub != getVaccin(), "Amount of vaccins in Hub must be updated");
    ENSURE(vaccinsCenter != center->getVaccins(), "Amount of vaccins in VaccinationCenter must be updated");
}

void Hub::print(std::ofstream &stream) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    stream << "Hub (" << getVaccin() << " vaccins)\n";

    // Traverse VaccinationCentra
    for(std::map<std::string,VaccinationCenter*>::const_iterator it = this->fcentra.begin(); it != this->fcentra.end(); it++){
        REQUIRE(it->second->properlyInitialized(), "VaccinationCenter must be properly initialized");
        stream << " -> " << it->first << " (" << (it->second)->getVaccins() << " vaccins)\n";
    }
    stream << "\n";
}

void Hub::insertVaccin(Vaccin* Vaccin) {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(Vaccin->properlyInitialized(), "Vaccin must be properly initialized");

    fvaccins.push_back(Vaccin);
}
