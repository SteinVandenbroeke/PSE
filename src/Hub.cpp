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

void Hub::copyHub(const Hub *h, const std::map<std::string, VaccinationCenter*> &centra) {

    REQUIRE(h->properlyInitialized(), "Hub must be properly initialized");

    for (std::map<std::string, VaccinInHub*>::const_iterator it = h->getVaccins().begin(); it != h->getVaccins().end(); it++) {

        VaccinInHub *v = new VaccinInHub();
        v->copyVaccin(it->second);

        this->fvaccins.insert(std::make_pair(v->getType(), v));
    }

    for (std::map<std::string, VaccinationCenter*>::const_iterator it = h->getCentra().begin(); it != h->getCentra().end(); it++) {

        this->fcentra.insert(std::make_pair(it->first, centra.find(it->first)->second));
    }
    this->_initCheck = this;
    ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state");
}

bool Hub::properlyInitialized() const {

    return Hub::_initCheck == this;
}

int Hub::getDelivery() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalDelivery = 0;
    for(std::map<std::string, VaccinInHub*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalDelivery += (*it).second->getDelivery();
    }
    return totalDelivery;
}

int Hub::getInterval() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalInterval = 0;
    for(std::map<std::string, VaccinInHub*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalInterval += (*it).second->getInterval();
    }
    return totalInterval;
}

int Hub::getTransport() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalTransport = 0;
    for(std::map<std::string, VaccinInHub*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        totalTransport += (*it).second->getTransport();
    }
    return totalTransport;
}

int Hub::getAmountVaccin() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    int totalVaccins = 0;

    for (std::map<std::string, VaccinInHub*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++) {
        totalVaccins += (*it).second->getVaccin();
    }
    return totalVaccins;
}

const std::map<std::string, VaccinationCenter*> &Hub::getCentra() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return fcentra;
}

void Hub::addCenter(const std::string &name, VaccinationCenter *center) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(center->properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(!this->containsVaccinationCenter(name) ,"VaccinationCenter must not exist yet in map");

    // Add center to map of Hub
    this->fcentra[name] = center;

    ENSURE( this->containsVaccinationCenter(name),"VaccinationCenter must exist in map");
}

void Hub::updateVaccins() {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    for(std::map<std::string, VaccinInHub*>::const_iterator it = fvaccins.begin(); it != fvaccins.end(); it++){
        (*it).second->updateVaccins();
    }
}

void Hub::distributeRequiredVaccins(VaccinationCenter* vaccinationCenter, std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(vaccinationCenter->properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(vaccinationCenter->properlyInitialized(), "VaccinationCenter must be properly initialized");

    vaccinationCenter->requiredAmountVaccinType();
    // Display information of transport
    std::map<std::string, int> requeredVaccins = vaccinationCenter->requiredAmountVaccinType();

    for (std::map<std::string, int>::iterator it = requeredVaccins.begin(); it != requeredVaccins.end(); it++) {

        if(fvaccins.find(it->first) != fvaccins.end()){

            VaccinInHub* vaccin = fvaccins[it->first];
            int vaccinsNeeded = it->second;

            int cargo = ceil((double)(vaccinsNeeded) / (double)(vaccin->getTransport()));

            while(vaccinationCenter->getOpenVaccinStorage(vaccin) < vaccin->getTransport() * cargo || cargo * vaccin->getTransport() > vaccin->getVaccin())
            {
                cargo--;
            }

            int vaccinsTransport = cargo * vaccin->getTransport();
            vaccin->updateVaccinsTransport(vaccinsTransport);
            vaccinationCenter->addVaccins(vaccinsTransport, vaccin);

            if (cargo == 0 && vaccinsTransport == 0) {
                return;
            }

            stream << "Er werden " << cargo << " nodige (voor 2de vaccinatie) ladingen (" << vaccinsTransport << " vaccins)" << " van " << vaccin->getType() << " getransporteerd naar ";
            stream << vaccinationCenter->getName() << ".\n";
        }
    }
}

void Hub::distributeVaccinsFair(VaccinInHub* vaccin, int currentDay, std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(currentDay >= 0, "currentDay cannot be negative");
    REQUIRE(vaccin->properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(containsVaccin(vaccin), "Given vaccin must exist");

    std::map<VaccinationCenter*, std::pair<int,int> > vaccinationCenterCargoTransport;
    int maxVaccinDeliveryDay = (vaccin->getVaccin())/(vaccin->getInterval() - (currentDay%vaccin->getInterval()));

    int vaccinsTransport = vaccin->getTransport();
    for(int i = 0; i <= maxVaccinDeliveryDay; i+=vaccinsTransport){
        if(vaccin->getVaccin() >= vaccin->getTransport()){

            VaccinationCenter* center = mostSuitableVaccinationCenter(vaccin->getTransport(), vaccin);

            if (center == NULL) {
                continue;
            }

            if(center->properlyInitialized()){
                if(vaccinationCenterCargoTransport.find(center) == vaccinationCenterCargoTransport.end()){
                    vaccinationCenterCargoTransport[center] = std::make_pair(0,0);
                }

                if (center->totalWaitingForSeccondPrik() + center->getVaccinated() == center->getPopulation()) {
                    continue;
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
        int vaccinTransport = it->second.second;

        stream << "Er werden " << cargo << " ladingen (" << vaccinTransport << " vaccins)" << " van " << vaccin->getType() << " getransporteerd naar ";
        stream << it->first->getName() << ".\n";
    }
}

void Hub::addVaccin(VaccinInHub* vaccin) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(vaccin->properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(!containsVaccin(vaccin), "Vaccin can't yet exist in Hub");

    this->fvaccins.insert(std::make_pair(vaccin->getType(), vaccin));

    ENSURE(containsVaccin(vaccin), "Vaccin must be added to Hub");
}

const std::map<std::string, VaccinInHub*> &Hub::getVaccins() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return fvaccins;
}

std::map<std::string, VaccinInHub *> &Hub::getVaccins() {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
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

const Hub::stockToSizeReturn Hub::stockToSize() const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    //Min size
    if (this->getAmountVaccin() == 0) {
        return stockToSizeReturn(0.1125, 0.1575, 8, 0.1125);
    }
    else {
        const double fac = static_cast<double>(ToPercent(this->getAmountVaccin(), this->getDelivery())) / 100;

        double cubeScale = 0.1125 * fac + 0.1125;
        if (cubeScale > 0.3375) {
            cubeScale = 0.3375;
        }
        double coneScale = 0.1575 * fac + 0.1575;
        if (coneScale > 0.4725) {
            coneScale = 0.4725;
        }
        int coneN = static_cast<int>(round(8 * fac)) + 8;
        if (coneN > 32) {
            coneN = 32;
        }
        return stockToSizeReturn(cubeScale, coneScale, coneN, cubeScale);
    }
}

std::string Hub::generateIni(std::ofstream &stream, int & counterFigures, int & counterHub) const {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");

    std::string x;

    Hub::stockToSizeReturn data = stockToSize();

    x.append("[Figure" + ToString(counterFigures) + "]" + "\n");
    x.append("type = \"Cube\"\n");
    x.append("scale = " + ToString(data.cubeScale) + "\n");
    x.append("rotateX = 0\n");
    x.append("rotateY = 0\n");
    x.append("rotateZ = 0\n");
    double positionX = static_cast<double>(counterHub) / 1.2;
    x.append("center = (" + ToString(positionX) + ", 0, 0)\n");
    x.append("color = (0, 0.6, 0.3)\n");
    counterFigures++;
    counterHub++;

    x.append("\n");

    x.append("[Figure" + ToString(counterFigures) + "]" + "\n");
    x.append("type = \"Cone\"\n");
    x.append("scale = " + ToString(data.coneScale) + "\n");
    x.append("n = " + ToString(data.coneN) + "\n");
    x.append("height = 1\n");
    x.append("rotateX = 0\n");
    x.append("rotateY = 0\n");
    x.append("rotateZ = 0\n");
    x.append("center = (" + ToString(positionX) + ", 0, " + ToString(data.coneCenterZ) + ")\n");
    x.append("color = (0, 0.6, 0.4)\n");
    counterFigures++;
    counterHub++;

    x.append("\n");

    ENSURE(x != "", "ini file can not be empty!");
    stream << x;
    return "(" + ToString(positionX) + ", 0, 0)\n";
}

VaccinationCenter *Hub::mostSuitableVaccinationCenter(int vaccinCount, VaccinInHub* vaccin) {

    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    REQUIRE(vaccin->properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(vaccinCount >= 0, "vaccinCount cannot be negative");
    REQUIRE(containsVaccin(vaccin), "Vaccin must exist in Hub");

    VaccinationCenter* center = this->getCentra().begin()->second;
    bool centerReached = false;
    for (std::map<std::string, VaccinationCenter *>::const_iterator it = this->getCentra().begin(); it != this->getCentra().end(); it++) {
        //selecteer het centrum met het miste vaccins tov de capaciteit

        if(((((double)(it->second->getVaccinated()) + (double)(it->second->getVaccins()))/(double)(it->second->getPopulation()) <= ((double)(center->getVaccinated())+ (double)(center->getVaccins()))/(double)(center->getPopulation()))
        || !centerReached) && (it->second->getOpenVaccinStorage(vaccin) >= vaccinCount)){
            center = it->second;
            centerReached = true;
        }
    }
    if(centerReached) {
        return center;
    }
    return NULL;
}

Hub::~Hub() {
    for(std::map<std::string, VaccinInHub*>::iterator vaccins = fvaccins.begin(); vaccins != fvaccins.end(); vaccins++){
        delete vaccins->second;
    }
    fvaccins.clear();
}

bool Hub::containsVaccinationCenter(const std::string &name) const {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return fcentra.find(name) != fcentra.end();
}

bool Hub::containsVaccin(const VaccinInHub* vaccin) {
    REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    return getVaccins().find(vaccin->getType()) != getVaccins().end();
}
