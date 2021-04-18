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

int Hub::getTotalDelivery() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalDeliveries = 0;

    for (std::map<std::string, Vaccin*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++) {
        totalDeliveries += (*it).second->getDelivery();
    }
    return totalDeliveries;
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

std::map<std::string, Vaccin*> Hub::getVaccinZero() {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    std::map<std::string, Vaccin*> vaccinUnderZero;
    for (std::map<std::string, Vaccin*>::iterator it = fvaccins.begin(); it != fvaccins.end(); it++) {

        if (it->second->getTemperature() < 0) {
            vaccinUnderZero.insert(*it);
        }
    }
    return vaccinUnderZero;
}

void Hub::transportVaccin(const std::string &centerName, int currentDay, std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(this->getCentra().find(centerName) != this->getCentra().end(), "Given centerName must exist");
    VaccinationCenter* center = fcentra[centerName];
    REQUIRE(center->getName() == centerName, "Name of found center and given center name must be equal");

    int vaccinsHub = getAmountVaccin();
    int vaccinsCenter = center->getVaccins();

    int vaccinsTransport = 0;
    int cargoTotal = 0;
    int cargo = 0;

    std::map<std::string, Vaccin*> underZeroVaccins = this->getVaccinZero();

    for (std::map<std::string, Vaccin*>::iterator it = fvaccins.begin(); it != fvaccins.end(); it++) {

        bool zeroVaccin = false;
        if(underZeroVaccins.find(it->first) != underZeroVaccins.end()){
            zeroVaccin = true;
        }

        Vaccin* vaccin = NULL;
        vaccin = it->second;

        vaccinsTransport = vaccin->calculateTransport(center, this->VaccinCentraCapacityRatio(center),currentDay, zeroVaccin);
        cargo = vaccinsTransport / vaccin->getTransport();
        cargoTotal += cargo;

        // Subtract transported amount of Vaccin and add to Center
        vaccin->updateVaccinsTransport(vaccinsTransport);
        center->addVaccins(vaccinsTransport, vaccin);

        // Display information of transport
        stream << "Er werden " << cargo << " ladingen (" << vaccinsTransport << " vaccins)" << " van " << it->first << " getransporteerd naar ";
        stream << center->getName() << ".\n";
    }

    // Display nothing
    if (cargoTotal == 0) {
        return;
    }

    ENSURE(vaccinsHub != this->getAmountVaccin(), "Amount of vaccins in Hub must be updated");
    ENSURE(vaccinsCenter != center->getVaccins(), "Amount of vaccins in VaccinationCenter must be updated");
}

void Hub::distributeRequeredVaccins(VaccinationCenter* vaccinationCenter, std::ostream &stream) {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(vaccinationCenter->properlyInitialized(), "VaccinationCenter must be properly initialized");

    vaccinationCenter->requiredAmountVaccinType();
    // Display information of transport
    std::map<std::string, int> requeredVaccins = vaccinationCenter->requiredAmountVaccinType();
    for (std::map<std::string, int>::iterator it = requeredVaccins.begin(); it != requeredVaccins.end(); it++) {

        if(fvaccins.find(it->first) != fvaccins.end()){
            Vaccin* vaccin = fvaccins[it->first];
            int vaccinsNeeded = it->second;

            int cargo = ceil(vaccinsNeeded/vaccin->getTransport());

            while(vaccinationCenter->getOpenVaccinStorage(vaccin) < vaccin->getTransport() * cargo)
            {
                cargo--;
            }
            int vaccinsTransport = cargo * vaccin->getTransport();
            vaccin->updateVaccinsTransport(vaccinsTransport);
            vaccinationCenter->addVaccins(vaccinsTransport, vaccin);

            stream << "(required) Er werden " << cargo << " ladingen (" << vaccinsTransport << " vaccins)" << " van " << vaccin->getType() << " getransporteerd naar ";
            stream << vaccinationCenter->getName() << ".\n";
        }
        else{
            std::cerr << "FOUT" << std::endl;
        }

    }
}

void Hub::distributeVaccinsFair(Vaccin* vaccin, int currentDay, std::ostream &stream) {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(currentDay >= 0, "currentDay cannot be negative");
    REQUIRE(this->getVaccins().find(vaccin->getType()) != this->getVaccins().end(), "Given vaccin must exist");

    std::map<VaccinationCenter*, std::pair<int,int> > vaccinationCenterCargoTransport;
    int maxVaccinDeliveryDay = (vaccin->getVaccin())/(vaccin->getInterval() - (currentDay%vaccin->getInterval()));

    int vaccinsTransport = vaccin->getTransport();
    for(int i = 0; i <= maxVaccinDeliveryDay; i+=vaccinsTransport){
        if(vaccin->getVaccin() >= vaccin->getTransport()){
            VaccinationCenter* center = mostSuitableVaccinationCenter(vaccin->getTransport(), vaccin);

            if(center->properlyInitialized()){
                if(vaccinationCenterCargoTransport.find(center) == vaccinationCenterCargoTransport.end()){
                    vaccinationCenterCargoTransport[center] = std::make_pair(0,0);
                }
                vaccinationCenterCargoTransport[center].first += 1;
                vaccinationCenterCargoTransport[center].second += vaccinsTransport;
                vaccin->updateVaccinsTransport(vaccinsTransport);
                center->addVaccins(vaccinsTransport, vaccin);
            }
        }
    }

    // Display information of transport
    for (std::map<VaccinationCenter*, std::pair<int,int> >::iterator it = vaccinationCenterCargoTransport.begin(); it != vaccinationCenterCargoTransport.end(); it++) {
        int cargo = it->second.first;
        int vaccinsTransport = it->second.second;
        stream << "Er werden " << cargo << " ladingen (" << vaccinsTransport << " vaccins)" << " van " << vaccin->getType() << " getransporteerd naar ";
        stream << it->first->getName() << ".\n";
    }
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
        it->second->printVaccins(stream);
    }
}

const Hub::stockToSizeReturn Hub::stockToSize() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    // Min size
    if (this->getAmountVaccin() == 0) {
        return stockToSizeReturn(0.1125, 0.1575, 8, 0.1125);
    }
    else {
        const double fac = static_cast<double>(ToPercent(this->getAmountVaccin(), this->getDelivery())) / 100;
        return stockToSizeReturn(0.1125 * fac + 0.1125, 0.1575 * fac + 0.1575,
                                 static_cast<int>(std::round(8 * fac)) + 8, 0.1125 * fac + 0.1125);
    }
}

std::string Hub::generateIni(std::ofstream &stream, int & counterFigures, int & counterHub) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    std::string x;

    Hub::stockToSizeReturn data = stockToSize();

    x.append("[Figure" + ToString(counterFigures) + "]" + "\n");
    x.append("type = \"Cube\"\n");
    x.append("scale = " + std::to_string(data.cubeScale) + "\n");
    x.append("rotateX = 0\n");
    x.append("rotateY = 0\n");
    x.append("rotateZ = 0\n");
    double positionX = static_cast<double>(counterHub) / 1.2;
    x.append("center = (" + std::to_string(positionX) + ", 0, 0)\n");
    x.append("color = (0, 0.6, 0.3)\n");
    counterFigures++;
    counterHub++;

    x.append("\n");

    x.append("[Figure" + ToString(counterFigures) + "]" + "\n");
    x.append("type = \"Cone\"\n");
    x.append("scale = " + std::to_string(data.coneScale) + "\n");
    x.append("n = " + std::to_string(data.coneN) + "\n");
    x.append("height = 1\n");
    x.append("rotateX = 0\n");
    x.append("rotateY = 0\n");
    x.append("rotateZ = 0\n");
    x.append("center = (" + std::to_string(positionX) + ", 0, " + std::to_string(data.coneCenterZ) + ")\n");
    x.append("color = (0, 0.6, 0.4)\n");
    counterFigures++;
    counterHub++;

    x.append("\n");

    stream << x;
    return "(" + std::to_string(positionX) + ", 0, 0)\n";
}

int Hub::totalVaccinCentraCapacity() {
    int totalCapacity = 0;
    for(std::map<std::string, VaccinationCenter*>::const_iterator it = fcentra.begin(); it != fcentra.end(); it++){
        totalCapacity += (*it).second->getCapacity();
    }
    return totalCapacity;
}

double Hub::VaccinCentraCapacityRatio(VaccinationCenter* center) {
    return (double)(center->getCapacity())/(double)(this->totalVaccinCentraCapacity());
}

VaccinationCenter *Hub::mostSuitableVaccinationCenter(int vaccinCount, Vaccin* vaccin) {
    VaccinationCenter* center = this->getCentra().begin()->second;
    bool centerReached = false;
    for (std::map<std::string, VaccinationCenter *>::iterator it = this->getCentra().begin(); it != this->getCentra().end(); it++) {
        //selecteer het centrum met het miste vaccins tov de capaciteit

        if(((((double)(it->second->getVaccinated()) + (double)(it->second->getVaccins()))/(double)(it->second->getPopulation()) <= ((double)(center->getVaccinated())+ (double)(center->getVaccins()))/(double)(center->getPopulation()))
        || !centerReached) && (it->second->getOpenVaccinStorage(vaccin) >= vaccinCount)){
            center = it->second;
            centerReached = true;
        }
    }
    if(centerReached){
        return center;
    }
}
