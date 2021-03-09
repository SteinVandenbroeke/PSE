/**
 * @file Simulation.cpp
 * @brief This file contains the definitions of the members of the Simulation class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#include "Simulation.h"


Simulation::Simulation() {

    _initCheck = this;
}

bool Simulation::properlyInitialized() const {

    return Simulation::_initCheck == this;
}

void Simulation::importXmlFile(const char *path) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");
    REQUIRE(FileExists(path), "The file that needs to be read must exist");
    REQUIRE(!FileIsEmpty(path), "THe file that needs to be read must not be empty");

    XMLReader reader(path);

    // Load hub in
    try{
        TiXmlElement* xmlHub = reader.getElement("HUB");
        int delivery = atoi(reader.getElementValue(*xmlHub, "levering"));
        int interval = atoi(reader.getElementValue(*xmlHub,"interval"));
        int transport = atoi(reader.getElementValue(*xmlHub,"transport"));

        // Create new hub
        fhub = new Hub(delivery, interval, transport);

        TiXmlElement* xmlCentra = xmlHub->FirstChildElement("CENTRA")->FirstChildElement("centrum");
        // TODO ksnap ni zo goe waarom ge dit er bij doet als ge da iets verder toch terug vervangt
//        while(xmlCentra != NULL){
//            try {
//                std::string name = xmlCentra->GetText();
////                fhub->addCenter(name, new VaccinationCenter());
//            }
//            // TODO
//            catch (Exception ex) {
//                cerr << ex.value() << endl;
//            }
//            xmlCentra = xmlCentra->NextSiblingElement("centrum");
//        }
    }
    catch (Exception ex) {
        cerr << ex.value() << endl;
    }

    if(this->fhub == NULL) throw Exception("Hub cannot be made, crucial information is missing");

    // Insert all vaccination centers into 'centra'
    TiXmlElement* xmlCentrum = reader.getElement("VACCINATIECENTRUM");
    while(xmlCentrum != NULL){
        try{
            std::string name = reader.getElementValue(*xmlCentrum,"naam");
            std::string address = reader.getElementValue(*xmlCentrum,"adres");
            int population = atoi(reader.getElementValue(*xmlCentrum,"inwoners"));
            int capacity = atoi(reader.getElementValue(*xmlCentrum,"capaciteit"));


            VaccinationCenter* center = new VaccinationCenter(name, address, population, capacity);

            this->fcentra.push_back(center);
            this->fhub->addCenter(center->getName(), center);
            // TODO
//            this->fhub->updateCenter(name, center);
        }
        catch (Exception ex) {
            cerr << ex.value() << endl;
        }
        xmlCentrum = xmlCentrum->NextSiblingElement("VACCINATIECENTRUM");
    }
}

void Simulation::exportFile(const char *path) {

    REQUIRE(properlyInitialized(), "Simulation object must be properly initialized");

    ofstream exportFile;
    exportFile.open(path);

    // Write hub data
    this->fhub->print(exportFile);

    // Traverse center
    for (int i = 0; i != this->fcentra.size(); i++) {
        // Write center data
        fcentra[i]->print(exportFile);
    }
    exportFile.close();

    ENSURE(FileExists(path), "File that has been written to must exist");
    ENSURE(!FileIsEmpty(path), "File that has been written to must not be empty");
}


