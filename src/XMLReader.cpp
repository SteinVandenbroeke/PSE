//
// Created by stein on 25/02/2021.
//

#include "XMLReader.h"

TiXmlElement* XMLReader::getElement(TiXmlElement& elmt, const char *name) {
    TiXmlElement* node = elmt.NextSiblingElement(name);
    if(node == NULL) throw Exception("Element not found: " + string(name));
    return node;
}

const char* XMLReader::getElementValue(TiXmlElement& elmt, const char *name) {
    TiXmlNode* node = elmt.FirstChild(name);
    if(node == NULL) throw Exception("Element not found: " + string(name));
    if(node->FirstChild() == NULL) throw Exception("Element is empty: " + string(name));

    TiXmlText* text = node->FirstChild()->ToText();
    if(text != NULL) return text->Value();
    throw Exception("Element not found " + string(name));
}

XMLReader::XMLReader(const char* filePad) {
    doc = new TiXmlDocument();
    if(!doc->LoadFile(filePad)) {
        throw Exception(doc->ErrorDesc());
    }
    if(doc->FirstChildElement() == NULL || doc->FirstChildElement()->FirstChildElement() == NULL) throw Exception("Wrong xml format");
}

TiXmlElement *XMLReader::getElement(const char *name) {
    TiXmlElement* node = doc->FirstChildElement(name);
    if(node == NULL) throw Exception("Element not found: " + string(name));
    return node;
}

XMLReader::~XMLReader() {
    doc->Clear();
    delete doc;
}
