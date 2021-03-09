/**
 * @file Simulation.h
 * @brief This header file contains the declarations and the members of the Simulation class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#ifndef TTT_SIMULATION_H
#define TTT_SIMULATION_H

#include <vector>
#include <iostream>
#include <fstream>
#include "XMLReader.h"
#include "VaccinationCenter.h"
#include "Hub.h"

/**
 * Class used to holds the simulation of different VaccinationCenters and Hubs
 */
class Simulation {

private:
    std::vector<VaccinationCenter*> fcentra;
    Hub* fhub;
    Simulation *_initCheck;

public:
    /**
    * \brief Imports a vaccin simulation from a .xml file
    *
    * @param path The path to the file that needs to be imported from
    *
    * @return A SuccessEnum, which could have one of the following values
    * ImportFailed: Something went wrong and we're not sure what has been changed to roadNetwork (This is an error)
    * ImportAborted: Nothing has been changed to your roadNetwork
    * PartialImport: There were a few errors in the xml file, so we had to leave some elements out to maintain a valid road situation
    * Succes: Everything from the file has been read and is added to the roadnetwork
    *
    * @pre
    * REQUIRE(roadNetwork->properlyInitialized(), "Roadnetwork moet juist geinitialiseerd zijn");
    * REQUIRE(roadNetwork->check(), "The roadnetwork must be valid");
    * REQUIRE(fileExists(filename), "Het bestand dat je wil inlezen moet bestaan");
    *
    * @post
    * ENSURE(roadNetwork->check(), "The roadnetwork is still valid");
    *
    */
    void importXmlFile(const char* path);

    void exportFile(const char* pad);
};

#endif //TTT_SIMULATION_H
