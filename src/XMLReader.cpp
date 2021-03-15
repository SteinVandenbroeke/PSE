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

XMLReader::XMLReader(const char* filePad) {

    doc = NULL;
    _initCheck = this;
    doc = new TiXmlDocument();
    if(!doc->LoadFile(filePad)) {
        throw Exception(doc->ErrorDesc());
    }
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
