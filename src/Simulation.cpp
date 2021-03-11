/**
 * @file Simulation.cpp
 * @brief This file contains the definitions of the members of the Simulation class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#include "Simulation.h"

Simulation::Simulation() {
    _initCheck = this;
    fhub = NULL;
    iter = 0;
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

Hub *Simulation::getFhub() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    return fhub;
}

void Simulation::importXmlFile(const char *path) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(FileExists(path), "The file that needs to be read must exist");
    REQUIRE(!FileIsEmpty(path), "THe file that needs to be read must not be empty");

    try{
        XMLReader reader(path);
        // Insert all vaccination centers into 'centra'
        TiXmlElement* xmlCentrum = reader.getElement("VACCINATIECENTRUM");
        while(xmlCentrum != NULL){
            try{
                std::string name = reader.getElementValue(*xmlCentrum,"naam");
                std::string address = reader.getElementValue(*xmlCentrum,"adres");
                int population = atoi(reader.getElementValue(*xmlCentrum,"inwoners"));
                int capacity = atoi(reader.getElementValue(*xmlCentrum,"capaciteit"));

                VaccinationCenter* center = new VaccinationCenter(name, address, population, capacity);

                this->fcentra[name] = center;
            }
            catch (Exception ex) {
                cerr << ex.value() << endl;
            }
            xmlCentrum = xmlCentrum->NextSiblingElement("VACCINATIECENTRUM");
        }

        // Load hub in
        try{
            TiXmlElement* xmlHub = reader.getElement("HUB");
            int delivery = atoi(reader.getElementValue(*xmlHub, "levering"));
            int interval = atoi(reader.getElementValue(*xmlHub,"interval"));
            int transport = atoi(reader.getElementValue(*xmlHub,"transport"));

            // Create new hub
            fhub = new Hub(delivery, interval, transport);

            TiXmlElement* xmlCentra = xmlHub->FirstChildElement("CENTRA")->FirstChildElement("centrum");
            while(xmlCentra != NULL){
                std::string name = xmlCentra->GetText();
                fhub->addCenter(name, this->fcentra[name]);
                xmlCentra = xmlCentra->NextSiblingElement("centrum");
            }
        }
        catch (Exception ex) {
            cerr << ex.value() << endl;
        }

        if(this->fhub == NULL) throw Exception("Hub cannot be made, crucial information is missing");


        ENSURE(checkSimulation(), "The simulation must be valid/consistent");
        ENSURE(fhub->getFvaccin() == fhub->getFdelivery()
        , "Hub must have equal amount of vaccins as delivery on day zero");
    }
    catch (Exception ex) {
        throw ex;
    }
}

bool Simulation::checkHub() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    return fhub != NULL;
}

bool Simulation::checkCentra() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    return !fcentra.empty();
}

bool Simulation::checkConnections() const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");

    // Check whether every VaccinationCenter is connected to the Hub
    if (fhub->getFcentra().size() != this->fcentra.size()) {
        return false;
    }

    // Traverse VaccinationCenters
    for (std::map<std::string, VaccinationCenter*>::const_iterator it = fcentra.begin(); it != fcentra.end(); it++) {

        // Search in hub for element with key "name"
        if (fhub->getFcentra().find(it->first) == fhub->getFcentra().end()) {
            return false;
        }
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

void Simulation::exportFile(const char *path) const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");

    ofstream exportFile;
    exportFile.open(path);

    // Write hub data
    fhub->print(exportFile);

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

    if (iter == 0) {
        REQUIRE(fhub->getFdelivery() == fhub->getFvaccin()
        , "Hub must have equal amount of vaccins as delivery on day zero");
    }

    // Interval between deliveries is over
    if (iter % fhub->getFinterval() == 0 && iter != 0) {
        stream << "Delivery!" << std::endl;
        fhub->updateVaccins();
    }

    // Traverse VaccinationCentra
    for (std::map<std::string, VaccinationCenter*>::iterator it = fcentra.begin(); it != fcentra.end(); it++) {

        //get name of first center connected to hub
        std::string centerName = fhub->getFcentra().find(it->first)->first;

        // Transport vaccins from hub to center
        fhub->transportVaccin(centerName, stream);
    }
    ENSURE(checkSimulation(), "The simulation must be valid/consistent");
    stream << "REMAINING: " <<fhub->getFvaccin() << std::endl;
}

void Simulation::simulateVaccination(std::ostream &stream) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");

    // Traverse VaccinationCentra
    for (std::map<std::string, VaccinationCenter*>::iterator it = fcentra.begin(); it != fcentra.end(); it++) {

        // Vaccinate in center
        it->second->vaccinateCenter(stream);
        stream << "HOLDING: " << it->second->getVaccins() << std::endl;
    }

    ENSURE(checkSimulation(), "The simulation must be valid/consistent");
}

void Simulation::increaseIterator() {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    iter ++;
}

void Simulation::automaticSimulation(int day, int month, int year, std::ostream &stream) {
    //day, month, year --> time_t
    struct tm * timeinfo;
    time_t rawtime;
    time(&rawtime);
    timeinfo = localtime ( &rawtime );
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day;
    time_t endDate = mktime(timeinfo);

    time_t current_date = time(0);
    while(current_date <= endDate){
        simulateTransport(stream);
        simulateVaccination(stream);

        exportFile("test.txt");
        increaseIterator();

        //time_t --> string
        std::tm * ptm = std::localtime(&current_date);
        char dateString[32];
        std::strftime(dateString, 32, "%d-%m-%Y", ptm);

        exportFile(dateString);//make export file

        current_date += 86400;// 1 dag toevoegen 24*60*60
    }
}
