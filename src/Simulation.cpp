/**
 * @file Simulation.cpp
 * @brief This file contains the definitions of the members of the Simulation class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#include "Simulation.h"

Simulation::Simulation() {
    fhub.clear();
    _initCheck = this;
    iter = 0;
}

Simulation::~Simulation() {

//    TODO
//
//    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
//
//    for (std::vector<Hub*>::iterator it = fhub.begin(); it != fhub.end(); it++) {
//
//        for (std::map<std::string, VaccinationCenter*>::iterator ite = (*it)->getCentra().begin();
//             ite != (*it)->getCentra().end(); ite++) {
//            if (ite->second != NULL) {
//                delete ite->second;
//            }
//        }
//
//        for (std::map<std::string, Vaccin*>::iterator ite = (*it)->getVaccins().begin();
//             ite != (*it)->getVaccins().end(); ite++) {
//            if (ite->second != NULL) {
//                delete ite->second;
//            }
//        }
//        delete (*it);
//    }
//    delete this;
}

bool Simulation::properlyInitialized() const {

    return Simulation::_initCheck == this;
}

int Simulation::getIter() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    return iter;
}

const std::map<std::string, VaccinationCenter*> &Simulation::getFcentra() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    return fcentra;
}

void Simulation::importXmlFile(const char *path, const char *knownTagsPad, std::ostream &errorStream) {

    XMLReader xmlReader = XMLReader(path);
    this->fcentra = xmlReader.readVaccinationCenters(errorStream);
    this->fhub = xmlReader.readHubs(this->fcentra, errorStream);

    ENSURE(checkSimulation(), "The simulation must be valid/consistent");
    ENSURE(checkVaccins(),"Hub must have equal amount of vaccins as delivery on day zero");
}

const std::vector<Hub *> &Simulation::getHub() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    return fhub;
}

bool Simulation::checkVaccins() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");

    for (std::vector<Hub*>::const_iterator it = this->fhub.begin(); it != this->fhub.end(); it++) {

        if ((*it)->getAmountVaccin() != (*it)->getDelivery()) {
            return false;
        }
    }
    return true;
}


bool Simulation::checkHub() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    return !this->fhub.empty();
}

bool Simulation::checkCentra() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    return !this->fcentra.empty();
}

bool Simulation::checkConnections() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkHub(), "Hub must exist");

    std::map<const std::string, bool> centraChecked;

    // Check whether every VaccinationCenter is connected to an existing Hub in the simulation and vica versa
    for (std::vector<Hub*>::const_iterator it = this->getHub().begin(); it != this->getHub().end(); it++) {

        const std::map<std::string, VaccinationCenter*> hubCentra = (*it)->getCentra();

        for (std::map<std::string, VaccinationCenter*>::const_iterator ite = hubCentra.begin(); ite != hubCentra.end(); ite++) {

            if (this->fcentra.find(ite->first) == this->fcentra.end()) {
                return false;
            }
            if (centraChecked.find(ite->first) == centraChecked.end()) {
                centraChecked.insert(std::make_pair(ite->first, true));
            }
        }
    }

    if (centraChecked.size() != this->fcentra.size()) {
        return false;
    }
    return true;
}

bool Simulation::checkSimulation() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");

    if (checkHub() && checkCentra() && checkConnections()) {
        return true;
    }
    return false;
}

void Simulation::exportFile(const std::string & path) const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");

    std::ofstream exportFile;
    exportFile.open(path.c_str());

    // Write hub data
    for (std::vector<Hub*>::const_iterator it = this->fhub.begin(); it != this->fhub.end(); it++) {

        (*it)->print(exportFile);
    }

    // Traverse VaccinationCenters
    for (std::map<std::string, VaccinationCenter*>::const_iterator it = fcentra.begin(); it != fcentra.end(); it++) {

        it->second->print(exportFile);
    }
    exportFile.close();

    ENSURE(FileExists(path), "File that has been written to must exist");
    ENSURE(!FileIsEmpty(path), "File that has been written to must not be empty");
}

void Simulation::simulateTransport(std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");

    for (std::vector<Hub*>::iterator ite = this->fhub.begin(); ite != this->fhub.end(); ite++) {

        for (std::map<std::string, VaccinationCenter*>::iterator it = fcentra.begin(); it != fcentra.end(); it++) {

            if ((*ite)->getCentra().find(it->first) == (*ite)->getCentra().end()) {
                continue;
            }

            std::string centerName = (*ite)->getCentra().find(it->first)->first;
            (*ite)->transportVaccin(centerName, stream);
        }
    }

    ENSURE(checkSimulation(), "The simulation must be valid/consistent");
}

void Simulation::simulateVaccination(std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");

    // Traverse VaccinationCentra
    for (std::map<std::string, VaccinationCenter*>::iterator it = fcentra.begin(); it != fcentra.end(); it++) {

        // Vaccinate in center
        it->second->vaccinateCenter(stream);
    }

    ENSURE(checkSimulation(), "The simulation must be valid/consistent");
}

void Simulation::increaseIterator() {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    iter ++;
}

void Simulation::automaticSimulation(const int days, std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");

    if (iter == 0) {
        ENSURE(checkVaccins(),"Hub must have equal amount of vaccins as delivery on day zero");
    }

    for(std::map<std::string, VaccinationCenter*>::iterator it = fcentra.begin(); it != fcentra.end();it++){
        REQUIRE(it->second->getVaccins() == 0 && it->second->getVaccinated() == 0,
                "Amount of vaccins or amount of vaccinated in a center must be 0 at begin of simulation");
    }

    while (iter < days) {

        for (std::vector<Hub*>::iterator it = fhub.begin(); it != fhub.end(); it++) {

            Hub* currentHub = (*it);

            for (std::map<std::string, Vaccin*>::iterator ite = currentHub->getVaccins().begin();
                    ite != currentHub->getVaccins().end(); ite++) {

                // Interval between deliveries is over
                if (iter % ite->second->getInterval() == 0 && iter != 0) {
                    ite->second->updateVaccins();
                }
            }
        }

        simulateTransport(stream);
        simulateVaccination(stream);
        exportFile("Day-" + ToString(iter));
        increaseIterator();
    }
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");
}
