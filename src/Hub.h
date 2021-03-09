/**
 * @file Hub.h
 * @brief This header file contains the declarations and the members of the Hub class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#ifndef TTT_HUB_H
#define TTT_HUB_H

#include <map>
#include "DesignByContract.h"
#include "VaccinationCenter.h"

/**
 * \brief Class implemented for a Hub
 */
class Hub {

private:
    int fdelivery; ///< Amount of vaccins delivered to Hub on the start and each interval
    int finterval; ///< Time between deliveries
    int ftransport; ///<  Amount of vaccins delivered to other VaccinationCenters
    int fvaccin; ///< Amount of vaccins currently in the hub
    std::map<std::string, VaccinationCenter*> fcentra; ///< Map with the connected VaccinationCenters
    Hub *_initCheck;

public:
    /**
     * \brief Non-default constructor for a Hub object
     *
     * @param fdelivery Amount of vaccins delivered to Hub
     * @param finterval Time between deliveries to Hub
     * @param ftransport Amount of vaccins in each transport to a VaccinationCenter
     *
     * @pre
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    Hub(int fdelivery, int finterval, int ftransport);

    /**
     * \brief Check whether the Hub object is properly initialised
     *
     * @return true when object is properly initialised, false when not
     */
    bool properlyInitialized() const;

    /**
     * \brief Add VaccinationCenter to map with connections of Hub
     *
     * @param name Name of the VaccinationCenter
     * @param center Pointer to VaccinationCenter object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(fcentra.find(name) == fcentra.end() ,"VaccinationCenter must not exist yet in map");
     *
     * @post
     * ENSURE( fcentra.find(name) != fcentra.end(),"VaccinationCenter must exist in map");
     */
    void addCenter(const std::string &name, VaccinationCenter* center);

    /**
     * \brief Update VaccinationCenter in map
     *
     * @param name Name of the VaccinationCenter
     * @param center Pointer to VaccinationCenter object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(fcentra.find(name) != fcentra.end() ,"VaccinationCenter must exist in map");
     *
     * @post
     * REQUIRE(fcentra.find(name) != fcentra.end(), "VaccinationCenter must exist in map");
     */
    void updateCenter(const std::string &name, VaccinationCenter* center);

    /**
     * \brief Print out data of Hub
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @post
     * TODO kweet eig ni of er een postconditie is
     */
    void print(std::ofstream& stream);
};

#endif //TTT_HUB_H
