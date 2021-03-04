//
// Created by stein on 25/02/2021.
//

#ifndef TTT_XMLREADER_H
#define TTT_XMLREADER_H
#include <string>
#include "xml/tinyxml.h"
#include "Exception.h"

using namespace std;

class XMLReader {
    TiXmlDocument* doc;
public:
    XMLReader(const char* filePad);
    TiXmlElement* getElement(TiXmlElement& elmt, const char *name);
    TiXmlElement* getElement(const char *name);
    const char* getElementValue(TiXmlElement& elmt, const char *name);
};


#endif //TTT_XMLREADER_H
