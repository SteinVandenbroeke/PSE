/**
 * @file Hub.h
 * @brief This header file contains the declarations and the members of the Hub class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#ifndef TTT_HUB_H
#define TTT_HUB_H

#include <map>
#include <vector>
#include "DesignByContract.h"
#include "VaccinationCenter.h"
#include "Vaccin.h"

/**
 * \brief Class implemented for a Hub
 */
class Hub {

private:
    std::map<std::string, Vaccin*> fvaccins; ///< Map with pointers to Vaccins of hub
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
     * REQUIRE(delivery >= 0, "Delivery can't be negative");
     * REQUIRE(interval >= 0, "Interval can't be negative");
     * REQUIRE(transport >= 0, "Transport can't be negative");
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    Hub();

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
     * \brief Get total amount of Vaccins stored in Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Amount of vaccins as int
     */
    int getAmountVaccin() const;

    /**
     * \brief Get vaccins of Hub object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Map with name of Vaccin as key and pointer to Vaccin object as value
     */
    const std::map<std::string, Vaccin *> &getVaccins() const;

    /**
     * \brief Get vaccins of Hub object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Map with name of Vaccin as key and pointer to Vaccin object as value
     */
    std::map<std::string, Vaccin *> &getVaccins();

    /**
     * \brief Add vaccin to Hub object
     *
     * @param Vaccin Pointer to Vaccin object to add
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(getVaccins().find(vaccin->getType()) == getVaccins().end(), "Vaccin can't yet exist in Hub")
     *
     * @post
     * ENSURE(getVaccins().find(vaccin->getType()) != getVaccins().end(), "Vaccin must be added to Hub")
     */
    void addVaccin(Vaccin* Vaccin);

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
     * \brief Get map with connected VaccinationCenters to Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Map with connected VaccinationCenters to Hub
     */
    std::map<std::string, VaccinationCenter *> &getCentra();

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
     */
    void updateVaccins();

    /**
     * \brief Calculate how many cargo's there will be transported to a Center
     *
     * @param center Pointer to the VaccinationCenter which transport shall be calculated
     * @param vaccin Pointer to Vaccin
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @post
     * ENSURE(vaccinsTransport <= vaccin->getVaccin(), "Amount of vaccinsTransport is too high")
     * ENSURE((vaccinsTransport <= (2 * center->getCapacity())), "Amount of vaccinsTransport is too high")
     *
     * @return Amount of cargo that will be transported as int
     */
    int calculateTransport(const VaccinationCenter* center, const Vaccin * vaccin) const;

    /**
     * \brief Transport vaccin from Hub to VaccinationCenter and display information if needed
     *
     * @param centerName Name of the VaccinationCenter where will be transported to
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(center->getName() == centerName, "Name of found center and given center name must be equal")
     * REQUIRE(fcentra.find(centerName) != fcentra.end(), "Given centerName must exist")
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

    /**
     *\brief Print out data of each vaccinationCenter with progressBar
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     */
    void printGraphical(std::ostream& stream) const;
};

#endif //TTT_HUB_H
