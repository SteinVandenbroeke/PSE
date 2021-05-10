/**
 * @file Simulation.h
 * @brief This header file contains the declarations and the members of the Simulation class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#ifndef TTT_SIMULATION_H
#define TTT_SIMULATION_H

#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <unistd.h>
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
    std::vector<Hub*> fhub; ///< Vector containing pointers to Hub object
    int iter;               ///< Iterator that holds the amount of iterations in the Simulation
    std::stack<Simulation*> undoStack; ///< Stack that holds the previous simulations
    Simulation *_initCheck;

    /**
     * \brief Increase iterator value
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     */
    void increaseIterator();

public:
    /**
     * \brief Default constructor for a Simulation object
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    Simulation();

    /**
     * \brief Deconstructor for a Simulation object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     */
    ~Simulation();

    /**
     * \brief "Copy constructor" for a Simulation object
     *
     * @param s Object to be copied from
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @post
     * ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state")
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     */
    Simulation(const Simulation &s);

    /**
     * \brief "Copy constructor" for a Simulation object
     *
     * @param s Object to be copied from
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @post
     * ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state")
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     */
    void copySimulation(const Simulation *s);

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
     * @return Vector with pointers to VaccinationCenters
     */
    const std::map<std::string, VaccinationCenter*> &getFcentra() const;

    /**
     * \brief Get Hubs in simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return Vector containing pointers to Hub object
     */
    const std::vector<Hub *> &getHub() const;

    /**
     * \brief Get undoStack
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return Stack containing all the previous Simulations
     */
    const std::stack<Simulation *> &getUndoStack() const;

    /**
     * \brief Imports a vaccin distribution simulation from a .xml file
     *
     * @param path The path to the .xml file that needs to read from
     * @param knownTagsPad Path to known tags of .xml file
     * @param errorStream Output-stream for errors, std = std::cerr
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(FileExists(path), "The file that needs to be read must exist")
     * REQUIRE(!FileIsEmpty(path), "The file that needs to be read must not be empty")
     *
     * @post
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     * ENSURE(checkVaccins(),"Hub must have equal amount of vaccins as delivery on day zero")
     */
    void importXmlFile(const char* path,const char *knownTagsPad="", std::ostream &errorStream = std::cerr);

    /**
     * \brief Check if all Hubs have an equal amount of vaccins to their respective amount of deliveries of each Vaccin
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * return True if the above statement is correct
     */
    bool checkVaccins() const;

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
     * REQUIRE(fhub != NULL, "Hub must exist")
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

    /**
     * \brief Export all the information of the Simulation to a .txt file
     *
     * @param path The path to the file that will be written to
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
     *
     * @post
     * ENSURE(FileExists(), "File that has been written to must exist")
     * ENSURE(!FileIsEmpty(path), "File that has been written to must not be empty")
     *
     */
    void exportFile(const std::string & path) const;

    /**
     * \brief Generate a .ini file of the current situation the Simulation is in
     *
     * @param path The path to the file will be written to
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
     *
     * @post
     * ENSURE(FileExists(), "File that has been written to must exist")
     * ENSURE(!FileIsEmpty(path), "File that has been written to must not be empty")
     */
    void generateIni(const std::string & path) const;

    /**
     * \brief Simulate transport of vaccins between Hub and centra
     *
     * @param stream Output-stream
     * @param currentDay Current day of simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
     *
     * @post
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     */
    void simulateTransport(int currentDay, std::ostream &stream);

    /**
    * \brief Simulate vaccination in centra
    *
    * @param stream Output-stream
    *
    * @pre
    * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
    * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
    *
    * @post
    * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
    */
    void simulateVaccination(std::ostream &stream);

    /**
     * \brief Simulation for amount of days
     *
     * @param days Amount of days needed to be simulated
     * @param stream Output-stream
     * @param export Export a file with data every day
     * @param ini Export a .ini file every day
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
     * ENSURE(checkVaccins(),"Hub must have equal amount of vaccins as delivery on day zero")
     * REQUIRE(days >= 0, "Days can't be negative");
     * REQUIRE(it->second->getVaccins() == 0 && it->second->getVaccinated() == 0,
                "Amount of vaccins or amount of vaccinated in a center must be 0 at begin of the simulation")
     *
     * @post
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     */
    void automaticSimulation(int days, std::ostream &stream, bool exportFlag, bool ini);

    /**
     * \brief Simulate for one day and generate .ini file
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
     * REQUIRE(this->iter >= 0, "Days can't be negative")
     *
     * @post
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     *
     * @return Pair of strings <Name of .ini file, output stream>
     */
    std::pair<std::string, std::string> simulate();

    /**
     * /brief Get total amount of persons vaccinated of all centra
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
     *
     * @post
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent");
     *
     * @return total in percent
     */
    int getVaccinatedPercent() const;

    /**
     * \brief Undo the simulation one day
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     * REQUIRE(checkSimulation(), "The simulation must be valid/consistent")
     *
     * @post
     * ENSURE(checkSimulation(), "The simulation must be valid/consistent")
     *
     * @return False if undoStack is empty, true if undo is success
     */
    bool undoSimulation();

    /**
     * \brief Clear simulation
     *
     * @param clearStack Clear undoStack with previous versions
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @post
     * ENSURE(getIter() == 0, "Iter must be zero")
     * ENSURE(getFcentra().empty(), "Centra must be empty")
     * ENSURE(getHub().empty(), "Hub must be empty")
     * if (clearStack) ENSURE(getUndoStack().empty(), "undoStack must be empty")
     */
    void clearSimulation(const bool clearStack);

    /**
     * \brief Get for each Vaccin in simulation total amount delivered Vaccins
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Simulation object must be properly initialized")
     *
     * @return Map containing data
     */
    std::map<const std::string, int> getVaccinData() const;
};

#endif //TTT_SIMULATION_H
