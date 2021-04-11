/**
 * @file XMLReader.cpp
 * @brief This file contains the definitions of the members of the XMLReader class
 * @author Stein Vandenbroeke
 * @date 25/02/2021
 */

#include "XMLReader.h"

const char* XMLReader::getElementValue(TiXmlElement& elmt, const char *name) {

    REQUIRE(properlyInitialized(), "XMLReader object must be properly initialized");
    TiXmlNode* node = elmt.FirstChild(name);
    if(node == NULL) throw Exception("Element not found: " + std::string(name));
    if(node->FirstChild() == NULL) throw Exception("Element is empty: " + std::string(name));

    TiXmlText* text = node->FirstChild()->ToText();
    if(text != NULL) return text->Value();
    throw Exception("Element not found " + std::string(name));
}

TiXmlElement *XMLReader::getElement(const char *name) {
    REQUIRE(properlyInitialized(), "XMLReader object must be properly initialized");

    TiXmlElement* node = doc->FirstChildElement(name);
    if(node == NULL) throw Exception("Element not found: " + std::string(name));
    return node;
}

XMLReader::~XMLReader() {

    doc->Clear();
    delete doc;
}

bool XMLReader::properlyInitialized() const {

    return XMLReader::_initCheck == this;
}

bool XMLReader::acceptedTags(std::ostream &errorStream, const char *knownTagsDocument) {
    REQUIRE(properlyInitialized(), "XMLReader object must be properly initialized");

    if(!FileExists(knownTagsDocument)){
        errorStream << "cannot find file with accepted tags" << std::endl;
    }
    TiXmlDocument* checkFile = new TiXmlDocument();
    if(!checkFile->LoadFile(knownTagsDocument)) {
        errorStream << "error in find file with accepted tags: " << checkFile->ErrorDesc() << std::endl;
    }

    TiXmlNode *node = checkFile->RootElement();
    allowedTags = new std::list<std::pair<std::string, int> >();
    knownTags(node,0);
    delete checkFile;
    node = doc->RootElement();
    return checkTags(node,errorStream,0);
}

bool XMLReader::checkTags(TiXmlNode *node, std::ostream &errorStream, int diepte) const {
    REQUIRE(properlyInitialized(), "XMLReader object must be properly initialized");

    bool rtn = true;
    while (node != NULL) {
        std::string naam = node->ToElement()->Value();
        if (!checkTag(naam, diepte)) {
            errorStream << "Error unknown tag: '" << node->ToElement()->Value() << "'" << std::endl;
            rtn = false;
        }
        if (node->FirstChildElement() != NULL) {
            bool rtn1 = checkTags(node->FirstChildElement(),errorStream,diepte + 1);
            if(rtn == true)
                rtn = rtn1;

        }
        node = node->Parent()->IterateChildren(node);
    }
    return rtn;
}

bool XMLReader::checkTag(std::string& name, int diepte) const {
    REQUIRE(properlyInitialized(), "XMLReader object must be properly initialized");

    for(std::list<std::pair<std::string, int> >::iterator it = allowedTags->begin(); it !=  allowedTags->end(); it++){
        if((*it).first == name && (*it).second == diepte){
            return true;
        }
    }
    return false;
}

void XMLReader::knownTags(TiXmlNode *node,int diepte) {
    REQUIRE(properlyInitialized(), "XMLReader object must be properly initialized");

    while (node != NULL) {
        std::string naam = node->ToElement()->Value();
        allowedTags->push_back(std::pair<std::string, int>(naam, diepte));
        if (node->FirstChildElement() != NULL) {
            knownTags(node->FirstChildElement(),diepte + 1);
        }
        node = node->Parent()->IterateChildren(node);
    }
}

std::vector<Hub*> XMLReader::readHubs(std::map<std::string, VaccinationCenter *> &vaccinationCentras,  std::ostream &errorStream) {
    REQUIRE(properlyInitialized(), "XMLReader object must be properly initialized");

    std::vector<Hub*> hubs;

    std::list<std::pair<std::string, int> > knownTags; //even = naam van tag, oneven = diepte van tag --> list[0] = naam en list[0 + 1] = diepte

    TiXmlElement* xmlHub = getElement("HUB");
    while(xmlHub != NULL) {

        Hub* newHub = new Hub();

        TiXmlElement* xmlVaccin = xmlHub->FirstChildElement("VACCIN");
        while (xmlVaccin != NULL) {
            std::string type = getElementValue(*xmlVaccin, "type");
            std::string delivery = getElementValue(*xmlVaccin, "levering");
            std::string interval = getElementValue(*xmlVaccin, "interval");
            std::string transport = getElementValue(*xmlVaccin, "transport");
            std::string renewal = getElementValue(*xmlVaccin, "hernieuwing");
            std::string temp = getElementValue(*xmlVaccin, "temperatuur");

            int intDelivery = ToInt(delivery);
            int intInterval = ToInt(interval);
            int intTransport = ToInt(transport);
            int intRenewal = ToInt(renewal);
            int intTemp = ToInt(temp);

            Vaccin* newVaccin = new Vaccin(type, intDelivery, intInterval, intTransport,
                                           intRenewal, intTemp);
            newHub->addVaccin(newVaccin);
            xmlVaccin = xmlVaccin->NextSiblingElement("VACCIN");
        }

        TiXmlElement* xmlCentra = xmlHub->FirstChildElement("CENTRA");
        TiXmlElement* xmlCenter = xmlCentra->FirstChildElement("centrum");
        while (xmlCenter != NULL) {
            std::string name = xmlCenter->GetText();
            if(vaccinationCentras.find(name) != vaccinationCentras.end()) {
                newHub->addCenter(name, vaccinationCentras[name]);
            }
            xmlCenter = xmlCenter->NextSiblingElement("centrum");
        }

        hubs.push_back(newHub);
        xmlHub = xmlHub->NextSiblingElement("HUB");
    }
    return hubs;
}

std::map<std::string, VaccinationCenter *> XMLReader::readVaccinationCenters(std::ostream &errorStream) {
    REQUIRE(properlyInitialized(), "XMLReader object must be properly initialized");
    std::map<std::string, VaccinationCenter *> VaccinationCentera;
    // Insert all vaccination centers into 'centra'
    TiXmlElement* xmlCentrum = getElement("VACCINATIECENTRUM");
    while(xmlCentrum != NULL) {
        try {
            std::string name = getElementValue(*xmlCentrum, "naam");
            std::string address = getElementValue(*xmlCentrum, "adres");
            std::string inwonersString = getElementValue(*xmlCentrum, "inwoners");
            std::string capacityString = getElementValue(*xmlCentrum, "capaciteit");
            int population = ToInt(inwonersString);
            int capacity = ToInt(capacityString);

            VaccinationCenter *center = new VaccinationCenter(name, address, population, capacity);

            VaccinationCentera[name] = center;
        }
        catch (Exception ex) {
            errorStream << ex.value() << std::endl;
        }
        xmlCentrum = xmlCentrum->NextSiblingElement("VACCINATIECENTRUM");
    }
    return VaccinationCentera;
}

XMLReader::XMLReader(const char *filePad) {
    REQUIRE(FileExists(filePad), "Cannot find file");
    doc = NULL;
    doc = new TiXmlDocument();
    if(!doc->LoadFile(filePad)) {
        throw Exception(doc->ErrorDesc());
    }
    _initCheck = this;
}

