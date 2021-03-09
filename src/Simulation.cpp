/**
 * @file Simulation.cpp
 * @brief This file contains the definitions of the members of the Simulation class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#include "Simulation.h"

/*
 * Load all information (Hub, centrums) in from XML file
 * @param pad: location to the XML file
 */
void Simulation::importXmlFile(const char *path) {

    XMLReader reader(pad);

    //load hub in
    try{
        TiXmlElement* xmlHub = reader.getElement("HUB");
        int delivery = atoi(reader.getElementValue(*xmlHub, "levering"));
        int interval = atoi(reader.getElementValue(*xmlHub,"interval"));
        int transport = atoi(reader.getElementValue(*xmlHub,"transport"));

        fhub = new Hub(delivery, interval, transport);

        TiXmlElement* xmlCentra = xmlHub->FirstChildElement("CENTRA")->FirstChildElement("centrum");
        while(xmlCentra != NULL){
            try {
                std::string name = xmlCentra->GetText();
                fhub->addCenter(name, new VaccinationCenter());
            }
            catch (Exception ex) {
                cerr << ex.value() << endl;
            }
            xmlCentra = xmlCentra->NextSiblingElement("centrum");

        }
    }
    catch (Exception ex) {
        cerr << ex.value() << endl;
    }

    if(this->fhub == NULL) throw Exception("Kan hub niet correct aanmaken, er ontbreken crusiale elementen.");

    //insert all vaccination centers into 'centra'
    TiXmlElement* xmlCentrum = reader.getElement("VACCINATIECENTRUM");
    while(xmlCentrum != NULL){
        try{
            string name = reader.getElementValue(*xmlCentrum,"naam");
            string address = reader.getElementValue(*xmlCentrum,"adres");
            int population = atoi(reader.getElementValue(*xmlCentrum,"inwoners"));
            int capacity = atoi(reader.getElementValue(*xmlCentrum,"capaciteit"));
            VaccinationCenter* center = new VaccinationCenter(name, address, population, capacity);
            this->fcentra.push_back(center);
            this->fhub->updateCenter(name, center);
        }
        catch (Exception ex) {
            cerr << ex.value() << endl;
        }
        xmlCentrum = xmlCentrum->NextSiblingElement("VACCINATIECENTRUM");
    }
}

/*
 * Create a file with all simulation information
 * @param pad: location for the export file
 */
void Simulation::exportFile(const char *pad) {

    ofstream exportFile;
    exportFile.open(pad);
    this->fhub->print(exportFile);
    int size =  this->fcentra.size();
    for(int i = 0; i < size; i++){
        this->fcentra[i]->print(exportFile);
    }
    exportFile.close();
}
