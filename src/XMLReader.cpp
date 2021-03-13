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
