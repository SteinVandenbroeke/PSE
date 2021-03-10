/**
 * @file Simulation.h
 * @brief This header file contains the declarations and the members of the Simulation class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#ifndef TTT_SIMULATION_H
#define TTT_SIMULATION_H

#include <map>
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
    std::map<std::string, VaccinationCenter*> fcentra; ///< vector with pointers to VaccinationCenter
    Hub* fhub; ///< Pointer to Hub object
    int iter; ///< Iterator that holds the amount of iterations in the Simulation
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
     * \brief Get amount of iterations of Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return Iteration-value of Simulation
     */
    int getIter() const;

    /**
     * \brief Get all VaccinationCenters in Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return  Vector with pointers to VaccinationCenters
     */
    const std::map<std::string, VaccinationCenter*> &getFcentra() const;

    /**
     * \brief Get Hub in Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return Pointer to Hub object
     */
    Hub *getFhub() const;

    /**
     * \brief Imports a vaccin distribution simulation from a .xml file
     *
     * @param path The path to the .xml file that needs to read from
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(FileExists(path), "The file that needs to be read must exist")
     * REQUIRE(!FileIsEmpty(path), "The file that needs to be read must not be empty")
     *
     *  TODO de simulatie moet consistent zijn
     *  TODO exceptions/uitzonderingen returnen
     *  TOOD weten of dat .xml file met tags enzo correct is
     *
     * @post
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     * ENSURE(fhub->getFvaccin() == fhub->getFdelivery(), "Hub must have equal amount of vaccins as delivery on day zero")
     * @Exceptions
     */
    void importXmlFile(const char* path);

    /**
     * \brief Check whether there is a Hub in the Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return True if there is a Hub present
     */
    bool checkHub() const;

    /**
     * \brief Check whether there is at least 1 VaccinationCenter in the Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return True if there is at least 1 VaccinationCenter present
     */
    bool checkCentra() const;

    /**
     * \brief Check whether every VaccinationCenter is connected to a Hub and
     *        every connection from a Hub to a VaccinationCenter is valid
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return True if the connections between VaccinationCenter and Hub are valid
     */
    bool checkConnections() const;

    /**
     * \brief Checks whether a Simulation is consistent with helper functions:
     *        -# checkHub()
     *        -# checkCentra()
     *        -# checkConnections()
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return True if Simulation is consistent
     */
    bool checkSimulation() const;

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
    void exportFile(const char* path) const;

    /*
     * \brief Simulate transport of vaccins between Hub and centra
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
     *
     * @post
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     */
    void simulateTransport();

    // TODO
    void simulateVaccination();

    /**
     * \brief Increase iterator value
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     */
    void increaseIterator();
};

#endif //TTT_SIMULATION_H
