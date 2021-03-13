/**
 * @file XMLReader.
 * @brief This header file contains the declarations and members of the XMLReader class
 * @author Stein Vandenbroeke
 * @date 25/02/2021
 */

#ifndef TTT_XMLREADER_H
#define TTT_XMLREADER_H

#include <string>
#include "xml/tinyxml.h"
#include "Exception.h"

/**
 * \brief Class implemented for a XMLReader object, used to read data from .xml files
 */
class XMLReader {

    TiXmlDocument* doc; // Pointer to TiXmlDocument
    XMLReader *_initCheck;

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

    /**
     * \brief Destroy XMLReader object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     */
    ~XMLReader();

    /**
    * \brief Check whether the XMLReader object is properly initialised
    *
    * @return true when object is properly initialised, false when not
    */
    bool properlyInitialized() const;

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
