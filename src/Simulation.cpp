/**
 * @file Simulation.cpp
 * @brief This file contains the definitions of the members of the Simulation class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#include "Simulation.h"

Simulation::Simulation() {

    _initCheck = this;
    fhub = NULL;
    iter = 0;
}

Simulation::~Simulation() {

    if(this->fhub != NULL){
        delete fhub;
    }
    for(std::map<std::string, VaccinationCenter*>::iterator it = fcentra.begin(); it != fcentra.end(); it++){
        if(it->second != NULL){
            delete it->second;
        }
    }
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

void Simulation::importXmlFile(const char *path, const char *knownTagsPad, std::ostream &errorStream) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(FileExists(path), "The file that needs to be read must exist");
    REQUIRE(!FileIsEmpty(path), "The file that needs to be read must not be empty");

    std::list<std::pair<std::string, int> > knownTags; //even = naam van tag, oneven = diepte van tag --> list[0] = naam en list[0 + 1] = diepte
    try{
        XMLReader reader(path);
        std::string knowTagsPadString = knownTagsPad;
        if(knowTagsPadString != "")
            reader.acceptedTags(errorStream, knownTagsPad);
        // Insert all vaccination centers into 'centra'
        TiXmlElement* xmlCentrum = reader.getElement("VACCINATIECENTRUM");
        while(xmlCentrum != NULL){
            try{
                std::string name = reader.getElementValue(*xmlCentrum,"naam");
                std::string address = reader.getElementValue(*xmlCentrum,"adres");
                std::string inwonersString = reader.getElementValue(*xmlCentrum,"inwoners");
                std::string capacityString = reader.getElementValue(*xmlCentrum,"capaciteit");
                int population = ToInt(inwonersString);
                int capacity = ToInt(capacityString);

                VaccinationCenter* center = new VaccinationCenter(name, address, population, capacity);

                this->fcentra[name] = center;
            }
            catch (Exception ex) {
                errorStream << ex.value() << std::endl;
            }
            xmlCentrum = xmlCentrum->NextSiblingElement("VACCINATIECENTRUM");
        }
        // Load hub in
        fhub = NULL;
        try{
            TiXmlElement* xmlHub = reader.getElement("HUB");
            std::string deliveryString = reader.getElementValue(*xmlHub, "levering");
            std::string intervalString = reader.getElementValue(*xmlHub, "interval");
            std::string transportString = reader.getElementValue(*xmlHub, "transport");
            int delivery = ToInt(deliveryString);
            int interval = ToInt(intervalString);
            int transport = ToInt(transportString);

            // Create new hub
            fhub = new Hub(delivery, interval, transport);

            TiXmlElement* xmlCentra = xmlHub->FirstChildElement("CENTRA")->FirstChildElement("centrum");
            while(xmlCentra != NULL){
                std::string name;
                if(xmlCentra->GetText() != NULL){
                    name = xmlCentra->GetText();
                    if(this->fcentra.find(name) != this->fcentra.end()){
                        fhub->addCenter(name, this->fcentra[name]);
                    }
                    else{
                        errorStream << "Hub contains an non existing or wrong vaccination center: '" << name << "'" << std::endl;
                    }
                }
                else{
                    errorStream << "Empty centra name!" << std::endl;
                }
                xmlCentra = xmlCentra->NextSiblingElement("centrum");
            }
        }
        catch (Exception ex) {
            std::cerr << ex.value() << std::endl;
        }

        if(this->fhub == NULL) {
            throw Exception("Hub cannot be made, crucial information is missing");
        }

        ENSURE(checkSimulation(), "The simulation must be valid/consistent");
        ENSURE(fhub->getVaccin() == fhub->getDelivery()
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
    REQUIRE(fhub != NULL, "Hub must exist");

    // Check whether every VaccinationCenter is connected to the Hub
    if (fhub->getCentra().size() != this->fcentra.size()) {
        return false;
    }

    // Traverse VaccinationCenters
    for (std::map<std::string, VaccinationCenter*>::const_iterator it = fcentra.begin(); it != fcentra.end(); it++) {

        // Search in hub for element with key "name"
        if (fhub->getCentra().find(it->first) == fhub->getCentra().end()) {
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

void Simulation::exportFile(const std::string & path) const {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");

    std::ofstream exportFile;
    exportFile.open(path.c_str());

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

    // Traverse VaccinationCentra
    for (std::map<std::string, VaccinationCenter*>::iterator it = fcentra.begin(); it != fcentra.end(); it++) {

        //get name of first center connected to hub
        std::string centerName = fhub->getCentra().find(it->first)->first;

        // Transport vaccins from hub to center
        fhub->transportVaccin(centerName, stream);
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
        REQUIRE(fhub->getDelivery() == fhub->getVaccin()
        , "Hub must have equal amount of vaccins as delivery on day zero");
    }

    for(std::map<std::string, VaccinationCenter*>::iterator it = fcentra.begin(); it != fcentra.end();it++){
        REQUIRE(it->second->getVaccins() == 0 && it->second->getVaccinated() == 0,
                "Amount of vaccins or amount of vaccinated in a center must be 0 at begin of simulation");
    }

    while (iter < days) {

        // Interval between deliveries is over
        if (iter % fhub->getInterval() == 0 && iter != 0) {
            fhub->updateVaccins();
        }

        simulateTransport(stream);
        simulateVaccination(stream);
        exportFile("Day-" + ToString(iter));
        increaseIterator();
    }

//    //day, month, year --> time_t
//    struct tm * timeinfo;
//    time_t rawtime;
//    time(&rawtime);
//    timeinfo = localtime ( &rawtime );
//    timeinfo->tm_year = year - 1900;
//    timeinfo->tm_mon = month - 1;
//    timeinfo->tm_mday = day;
//    time_t endDate = mktime(timeinfo);
//
//    time_t current_date = time(0);
//    while(current_date <= endDate){
//        simulateTransport(stream);
//        simulateVaccination(stream);
//        increaseIterator();
//
//        //time_t --> strings
//        std::tm * tm = std::localtime(&current_date);
//        char dateString[32];
//        std::strftime(dateString, 32, "%d-%m-%Y.txt", tm);
//        exportFile(dateString); //make export file
//        current_date += 86400; // 1 dag toevoegen 24*60*60
//    }


    REQUIRE(checkSimulation(), "The simulation must be valid/consistent");
}
