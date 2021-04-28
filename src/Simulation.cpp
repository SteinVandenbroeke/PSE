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

    for(std::map<std::string, VaccinationCenter*>::iterator centra = fcentra.begin(); centra != fcentra.end(); centra++){
        delete centra->second;
    }
    fcentra.clear();
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

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(FileExists(path), "The file that needs to be read must exist");
    REQUIRE(!FileIsEmpty(path), "The file that needs to be read must not be empty");

    XMLReader xmlReader = XMLReader(path);
    std::string empty = "";

    try{
        if(knownTagsPad != empty && knownTagsPad != NULL){
            xmlReader.acceptedTags(errorStream, knownTagsPad);
        }
        this->fcentra = xmlReader.readVaccinationCenters(errorStream);
        this->fhub = xmlReader.readHubs(this->fcentra, errorStream);
    }
    catch (Exception ex) {
        throw Exception(ex.value());
    }

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

void Simulation::generateIni(const std::string &path) const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");

    std::ofstream ini;
    ini.open(path.c_str());

    // Write general data to string
    std::string x;
    x.append("[General]\n");
    x.append("size = 1024\n");
    x.append("backgroundcolor = (0.169, 0.169, 0.169)\n");
    x.append("type = \"ZBuffering\"\n");
    x.append("eye = (200, 60, 70)\n");
    int amountFigures = (this->fhub.size() * 2 + 1) + this->fcentra.size();
    x.append("nrFigures = " + ToString(amountFigures) + "\n");
    x.append("\n");

    // Write to .ini file and clear string
    ini << x;
    x.clear();

    int counterFigures = 0; // Holds current figure number
    int counterHub = 0; // Holds current hub number
    double maxHubX = static_cast<double>(fhub.size() * 2) / 1.2; // x-position of last Hub
    int pointCounter = 0; // Holds current point number
    std::string centerPoints; // Holds data for position points of centra

    std::map<std::string, std::pair<double, double> > centerPositions;

    // Traverse center and write to string
    int counterCenter = 1;
    for (std::map<std::string, VaccinationCenter*>::const_iterator it = this->fcentra.begin(); it != this->fcentra.end(); it++) {

        std::pair<double, double> itPosition = it->second->generateIni(ini, counterFigures, counterCenter, maxHubX);
        centerPositions[it->first] = itPosition;
        // Write position data of center to string
        centerPoints.append("point" + ToString(pointCounter) + " = (" + ToString(itPosition.first) + ", ");
        centerPoints.append(ToString(itPosition.second) + ", 0)\n");
        pointCounter++;
    }

    int lineCounter = 0;
    int currentHub = pointCounter + fhub.size() - 1; // Number of point of current hub
    std::string hubPoints;
    std::string lines;

    for (std::vector<Hub*>::const_iterator it = this->fhub.begin(); it != this->fhub.end(); it++) {

        hubPoints.append("point" + ToString(pointCounter) + " = ");
        hubPoints.append((*it)->generateIni(ini, counterFigures, counterHub));
        pointCounter++;

        for (std::map<std::string, VaccinationCenter*>::const_iterator ite = (*it)->getCentra().begin(); ite != (*it)->getCentra().end(); ite++) {

            int index = distance(centerPositions.begin(), centerPositions.find(ite->first));

            lines.append("line" + ToString(lineCounter) + " = ");
            lines.append("(" + ToString(currentHub) + "," + ToString(index) + ")\n");
            lineCounter++;
        }
        currentHub--;
    }

    // Line drawing for all Hubs
    x.append("[Figure" + ToString(counterFigures) + "]" + "\n");
    x.append("type = \"LineDrawing\"\n");
    x.append("scale = 1.0\n");
    x.append("rotateX = 0\n");
    x.append("rotateY = 0\n");
    x.append("rotateZ = 0\n");
    x.append("center = (0, 0, 0)\n");
    x.append("color = (0, 0.4, 1.0)\n");

    x.append("nrPoints = " + ToString(pointCounter) + "\n");
    x.append("nrLines = " + ToString(lineCounter) + "\n");
    x.append(centerPoints);
    x.append(hubPoints);
    x.append(lines);

    ini << x;
    x.clear();
    ini.close();

    ENSURE(FileExists(path), "File that has been written to must exist");
    ENSURE(!FileIsEmpty(path), "File that has been written to must not be empty");
}

void Simulation::simulateTransport(int currentDay, std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");

    for (std::vector<Hub*>::iterator ite = this->fhub.begin(); ite != this->fhub.end(); ite++) {

        std::map<std::string, VaccinationCenter *> centra = (*ite)->getCentra();
        for (std::map<std::string, VaccinationCenter *>::iterator it = centra.begin(); it != centra.end(); it++) {
            (*ite)->distributeRequiredVaccins(it->second, stream);
        }
    }

    for (std::vector<Hub*>::iterator ite = this->fhub.begin(); ite != this->fhub.end(); ite++) {
        std::map<std::string, Vaccin *> vaccins = (*ite)->getVaccins();
        for (std::map<std::string, Vaccin *>::iterator it = vaccins.begin(); it != vaccins.end(); it++) {
            (*ite)->distributeVaccinsFair(it->second,currentDay, stream);
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
    iter++;
}

void Simulation::automaticSimulation(const int days, std::ostream &stream, bool exportFlag, bool ini) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");
    REQUIRE(days >= 0, "Days can't be negative");

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

        simulateTransport(iter, stream);
        simulateVaccination(stream);

        for (std::vector<Hub*>::iterator ite = this->fhub.begin(); ite != this->fhub.end(); ite++) {
            (*ite)->printGraphical(stream);
            std::map<std::string, VaccinationCenter *> centra = (*ite)->getCentra();
            for (std::map<std::string, VaccinationCenter*>::iterator it = centra.begin(); it != centra.end(); it++) {
                it->second->updateRenewal();
            }
        }

        if (exportFlag) {
            exportFile("Day-" + ToString(iter) + ".txt");
        }
        if (ini) {
            generateIni("Day-" + ToString(iter) + ".ini");
        }
        increaseIterator();
    }
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");
}
