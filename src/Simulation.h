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
#include "DesignByContract.h"
#include "Utils.h"
#include "VaccinationCenter.h"
#include "Hub.h"

/**
 * Class used to holds the simulation of different VaccinationCenters and Hubs
 */
class Simulation {

private:
    std::vector<VaccinationCenter*> fcentra; ///< Vector with pointers to VaccinationCenter
    Hub* fhub; ///< Pointer to Hub object
    Simulation *_initCheck;

public:
    /**
     * \brief Default constructor for a Simulation object
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    Simulation();

    /**
     * \brief Check whether the Simulation object is properly initialised
     *
     * @return true when object is properly initialised, false when not
     */
    bool properlyInitialized() const;

    /**
     * \brief Imports a vaccin distribution simulation from a .xml file
     *
     * @param path The path to the .xml file that needs to read from
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(FileExists(path), "The file that needs to be read must exist")
     * REQUIRE(!FileIsEmpty(path), "THe file that needs to be read must not be empty")
     *
     *  TODO de simulatie moet consistent zijn
     *  TODO exceptions/uitzonderingen returnen
     *  TOOD weten of dat .xml file met tags enzo correct is
     *
     * @post
     *
     *
     * @Exceptions
     */
    void importXmlFile(const char* path);

    /*
     * \brief Export all the information of the Simulation to a .txt file
     *
     * @param path The path to the file that will be written to
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @post
     * ENSURE(FileExists(), "File that has been written to must exist")
     * ENSURE(!FileIsEmpty(path), "File that has been written to must not be empty")
     *
     */
    void exportFile(const char* path);
};

#endif //TTT_SIMULATION_H
