//
// Created by stein on 4/03/2021.
//

#include "Vaccinatiecentra.h"

Vaccinatiecentra::Vaccinatiecentra() {
}

void Vaccinatiecentra::loadXml(const char* xmlPad) {
    XMLReader reader(xmlPad);
    TiXmlElement* xmlCentrum = reader.getElement("VACCINATIECENTRUM");

    while(xmlCentrum != NULL){
        string naam = reader.getElementValue(*xmlCentrum,"naam");
        string adres = reader.getElementValue(*xmlCentrum,"adres");
        string inwoners = reader.getElementValue(*xmlCentrum,"adres");
        std::string::size_type sz;
        int inwoners = stoi(inwoners, sz);
        int capaciteit = stoi(reader.getElementValue(*xmlCentrum,"adres"));
        Vaccinatiecentrum* centrum = new Vaccinatiecentrum(naam,adres,inwoners,capaciteit);
        this->centra.push_back(centrum);

        xmlCentrum = xmlCentrum->NextSiblingElement("VACCINATIECENTRUM");
    }
    int size = centra.size();
    for(int i = 0; i < size; i++){
        cout << "centrum " << (i + 1) << endl << centra[i]->getNaam() << endl<< centra[i]->getAdres() << endl<< centra[i]->getInwoners() << endl <<"-----" << endl;
    }
}
