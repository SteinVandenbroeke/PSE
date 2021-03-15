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
#include <list>
#include <iostream>
#include <algorithm>
#include "Utils.h"

/**
 * \brief Class implemented for a XMLReader object, used to read data from .xml files
 */
class XMLReader {

    TiXmlDocument* doc; // Pointer to TiXmlDocument
    XMLReader *_initCheck;
    std::list<std::pair<std::string, int> >* allowedTags;

    /**
     * \brief check's if the node with name 'name' is in the allowedTags list
     *
     * @param name: the name of the node
     * @param diepte: the nested depth of the node
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return bool true: node name is in list, false: node name is not in the list
     *
     */
    bool checkTag(std::string& name, int depht) const;

    /**
     * \brief check's for all sibling, child nodes of the name is in the list
     *
     * @param node: the start node
     * @param errorStream: the error stream to print
     * @param diepte: starting depth
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return bool true: node name is in list, false: node name is not in the list
     *
     */
    bool checkTags(TiXmlNode *node, std::ostream &errorStream, int depht) const;

    /**
     * \brief pushes all node names (sibling, child nodes) to the allowedTags list
     *
     * @param node: the start node
     * @param depht: starting depth
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     */
    void knownTags(TiXmlNode *node,int depht);
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

    /**
     * \brief checks if all nodes in the doc are allowd with a knownTags.xml document
     *
     * @param errorStream the error stream
     * @param knownTagsDocument the example file with all the allowed tags
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return true: all tags are allowed, false: not all the tags are allowed
     */
    bool acceptedTags(std::ostream &errorStream, const char *knownTagsDocument);
};

#endif //TTT_XMLREADER_H
