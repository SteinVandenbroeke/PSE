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
    TiXmlDocument* doc = NULL;
    XMLReader *_initCheck;
    bool properlyInitialized() const;
public:
    /**
     * \brief initialize XMLReader, en create a TiXmlDocument for filepad
     *
     * @param filePad location of the xml file
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     */
    XMLReader(const char* filePad);
    ~XMLReader();

    /**
     * \brief gives TiXmlElement whith tag 'name'
     *
     * @param name name of the element
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     */
    TiXmlElement* getElement(const char *name);

    /**
     * \brief gives the value of a tag with name 'name' in an TiXmlElement 'elmt'
     *
     * @param elmt the element where tag name exist's
     * @param name the name of the element
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return value of the element with name 'name' in TiXmlElement 'elm'
     */
    const char* getElementValue(TiXmlElement& elmt, const char *name);
};


#endif //TTT_XMLREADER_H
