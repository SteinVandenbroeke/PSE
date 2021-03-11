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
    int ftransport; ///< Amount of vaccins delivered to other VaccinationCenters
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
    Hub(int delivery, int interval, int transport);

    /**
     * \brief Check whether the Hub object is properly initialised
     *
     * @return true when object is properly initialised, false when not
     */
    bool properlyInitialized() const;

    /**
     * \brief Get amount of vaccins delivered to Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Amount of vaccins
     */
    int getDelivery() const;

    /**
     * \brief Get interval between deliveries
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Interval between deliveries as int
     */
    int getInterval() const;

    /**
     * \brief Get amount of vaccins delivered to VaccinationCenters
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Amount of vaccins delivered to VaccinationCenters as int
     */
    int getTransport() const;

    /**
     * \brief Get amount of vaccins currently in Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Amount of vaccins as int
     */
    int getVaccin() const;

    /**
     * \brief Get map with connected VaccinationCenters to Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Map with connected VaccinationCenters to Hub
     */
    const std::map<std::string, VaccinationCenter *> &getCentra() const;

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
     * \brief Update the amount of vaccins in Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     */
    void updateVaccins();

    /**
     * \brief Calculate how many cargo's there will be transported to a Center
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @post
     * TODO
     *
     * @return Amount of cargo that will be transported as int
     */
    int calculateTransport(const VaccinationCenter* center) const;

    /**
     * \brief Transport vaccin from Hub to VaccinationCenter and display information if needed
     *
     * @param centerName Name of the VaccinationCenter where will be transported to
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(center->getName() == centerName, "Name of found center and given center name must be equal")
     *
     * @post
     * ENSURE(vaccinsHub != fvaccin, "Amount of vaccins in Hub must be updated")
     * ENSURE(vaccinsCenter != center->getVaccins(), "Amount of vaccins in VaccinationCenter must be updated")
     */
    void transportVaccin(const std::string &centerName, std::ostream &stream);

    /**
     * \brief Print out data of Hub
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     */
    void print(std::ofstream& stream) const;
};

#endif //TTT_HUB_H
