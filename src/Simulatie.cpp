//
// Created by stein on 4/03/2021.
//

#include "Simulatie.h"

/*
 * Load all information (Hub, centrums) in from XML file
 * @param pad: location to the XML file
 */
void Simulatie::readXmlFile(const char *pad) {
    XMLReader reader(pad);

    //load hub in
    try{
        TiXmlElement* xmlHub = reader.getElement("HUB");
        int levering = atoi(reader.getElementValue(*xmlHub,"levering"));
        int interval = atoi(reader.getElementValue(*xmlHub,"interval"));
        int transport = atoi(reader.getElementValue(*xmlHub,"transport"));
        hub = new Hub(levering, interval, transport);

        TiXmlElement* xmlCentra = xmlHub->FirstChildElement("CENTRA")->FirstChildElement("centrum");
        while(xmlCentra != NULL){
            try {
                string naam = xmlCentra->GetText();
                hub->addVaccinatiecentrum(naam, new Vaccinatiecentrum());
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

    if(hub == NULL) throw Exception("Kan hub niet correct aanmaken, er ontbreken crusiale elementen.");

    //insert all vaccination centers into 'centra'
    TiXmlElement* xmlCentrum = reader.getElement("VACCINATIECENTRUM");
    while(xmlCentrum != NULL){
        try{
            string naam = reader.getElementValue(*xmlCentrum,"naam");
            string adres = reader.getElementValue(*xmlCentrum,"adres");
            int inwoners = atoi(reader.getElementValue(*xmlCentrum,"inwoners"));
            int capaciteit = atoi(reader.getElementValue(*xmlCentrum,"capaciteit"));
            Vaccinatiecentrum* centrum = new Vaccinatiecentrum(naam,adres,inwoners,capaciteit);
            centra.push_back(centrum);
            hub->updateVaccinatiecentrum(naam, centrum);
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
void Simulatie::exportFile(const char *pad) {
    ofstream exportFile;
    exportFile.open(pad);
    hub->print(exportFile);
    int size =  centra.size();
    for(int i = 0; i < size; i++){
        this->centra[i]->print(exportFile);
    }
    exportFile.close();
}
