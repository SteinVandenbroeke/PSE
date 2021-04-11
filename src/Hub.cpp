/**
 * @file Hub.cpp
 * @brief This file contains the definitions of the members of the Hub class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#include "Hub.h"

Hub::Hub(){

    fcentra.clear();
    fvaccins.clear();
    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool Hub::properlyInitialized() const {

    return Hub::_initCheck == this;
}

int Hub::getDelivery() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalDelivery = 0;
    for(std::map<std::string, Vaccin*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalDelivery += (*it).second->getDelivery();
    }
    return totalDelivery;
}

int Hub::getInterval() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalInterval = 0;
    for(std::map<std::string, Vaccin*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalInterval += (*it).second->getInterval();
    }
    return totalInterval;
}

int Hub::getTransport() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalTransport = 0;
    for(std::map<std::string, Vaccin*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalTransport += (*it).second->getTransport();
    }
    return totalTransport;
}

int Hub::getAmountVaccin() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalVaccins = 0;

    for (std::map<std::string, Vaccin*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++) {
        totalVaccins += (*it).second->getVaccin();
    }
    return totalVaccins;
}

const std::map<std::string, VaccinationCenter *> &Hub::getCentra() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return fcentra;
}

std::map<std::string, VaccinationCenter *> &Hub::getCentra() {

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
    for(std::map<std::string, Vaccin*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        (*it).second->updateVaccins();
    }
}

int Hub::calculateTransport(const VaccinationCenter* center, const Vaccin * vaccin) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    int cargo = 0;
    int vaccinsTransport = 0;

    if (vaccin->getTransport() == 0) {
        return vaccinsTransport;
    }

    while ((((vaccin->getTransport() * cargo) + center->getVaccins()) <= (2 * center->getCapacity())) &&
           (vaccin->getTransport() * cargo) <= vaccin->getVaccin()) {

        if ((vaccin->getTransport() * cargo) + center->getVaccins() > center->getCapacity()) {

            if (((vaccin->getTransport() * cargo) + center->getVaccins()) - center->getCapacity() < vaccin->getTransport()) {
                vaccinsTransport = cargo * vaccin->getTransport();
                cargo ++;
                break;
            }
            else {
                break;
            }
        }
        vaccinsTransport = cargo * vaccin->getTransport();
        cargo ++;
    }

    ENSURE(vaccinsTransport <= vaccin->getVaccin(), "Amount of vaccinsTransport is too high");
    ENSURE((vaccinsTransport <= (2 * center->getCapacity())), "Amount of vaccinsTransport is too high");
    return vaccinsTransport;
}


void Hub::transportVaccin(const std::string &centerName, std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(this->getCentra().find(centerName) != this->getCentra().end(), "Given centerName must exist");
    VaccinationCenter* center = fcentra[centerName];
    REQUIRE(center->getName() == centerName, "Name of found center and given center name must be equal");

    int vaccinsHub = getAmountVaccin();
    int vaccinsCenter = center->getVaccins();

    // Calculate cargo
    int vaccinsTransport = 0;

    for (std::map<std::string, Vaccin*>::const_iterator it = this->fvaccins.begin(); it != this->fvaccins.end(); it++) {

        vaccinsTransport += it->second->calculateTransport(center);
        center->addVaccins(vaccinsTransport);
        break;
    }
    int cargo = vaccinsTransport / getTransport();

    // Display nothing
    if (cargo == 0) {
        return;
    }

    // Update amount of vaccins in Hub and Center
//    fvaccin -= vaccinsTransport;
//    center->addVaccins(vaccinsTransport);

    // Display information of transport
    stream << "Er werden " << cargo << " ladingen (" << vaccinsTransport << " vaccins) getransporteerd naar ";
    stream << center->getName() << ".\n";

    std::cout << "lol" << std::endl;

    ENSURE(vaccinsHub != this->getAmountVaccin(), "Amount of vaccins in Hub must be updated");
    ENSURE(vaccinsCenter != center->getVaccins(), "Amount of vaccins in VaccinationCenter must be updated");
}


void Hub::addVaccin(Vaccin* vaccin) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(getVaccins().find(vaccin->getType()) == getVaccins().end(), "Vaccin can't yet exist in Hub");
    this->fvaccins.insert(std::make_pair(vaccin->getType(), vaccin));
    ENSURE(getVaccins().find(vaccin->getType()) != getVaccins().end(), "Vaccin must be added to Hub");
}

const std::map<std::string, Vaccin *> &Hub::getVaccins() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccins;
}

std::map<std::string, Vaccin *> &Hub::getVaccins() {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccins;
}

void Hub::print(std::ofstream &stream) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    stream << "Hub (" << getAmountVaccin() << " vaccins)\n";

    // Traverse VaccinationCentra
    for(std::map<std::string,VaccinationCenter*>::const_iterator it = this->fcentra.begin(); it != this->fcentra.end(); it++){
        REQUIRE(it->second->properlyInitialized(), "VaccinationCenter must be properly initialized");
        stream << " -> " << it->first << " (" << (it->second)->getVaccins() << " vaccins)\n";
    }
    stream << "\n";
}

void Hub::printGraphical(std::ostream &stream) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    for (std::map<std::string, VaccinationCenter*>::const_iterator it = this->fcentra.begin(); it != this->fcentra.end(); it++) {
        it->second->printGraphical(stream);
    }
}
