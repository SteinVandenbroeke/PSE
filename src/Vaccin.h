/**
 * @file Vaccin.h
 * @brief This header file contains the declarations and the members of the Vaccin class
 * @author Stein Vandenbroeke
 * @date 09/04/2021
 */

#ifndef VACCINDISTRIBUTOR_VACCIN_H
#define VACCINDISTRIBUTOR_VACCIN_H

#include "DesignByContract.h"
#include <string>
#include <fstream>
#include "VaccinationCenter.h"

class VaccinationCenter;

class Vaccin {

private:
    std::string ftype; ///< Type of the vaccin
    int fdelivery; ///< Amount of vaccins delivered to Hub on the start and each interval
    int finterval; ///< Time between deliveries
    int ftransport; ///< Amount of vaccins from this type delivered to other VaccinationCenters
    int fvaccin; ///< Amount of vaccins from this type currently in the hub
    int frenewal; ///< Amount of time between the first shot and second shot to aquire max. immunity, 0 = std
    int ftemperature; ///< Temperature where the vaccin should be stored
    Vaccin *_initCheck;

public:
    /**
     * \brief Non-default constructor for Vaccin type object
     *
     * @param type name / type of vaccin
     * @param delivery Amount of vaccins that can de delivered to Hub
     * @param interval Time between deliveries to Hub
     * @param transport Amount of vaccins in each transport to a VaccinationCenter
     * @param renewal Amount of time between vaccin shots
     * @param temp Temperature where the vaccin should be stored
     *
     * @pre
     * REQUIRE(type != "", "type can't be empty");
     * REQUIRE(delivery >= 0, "Delivery can't be negative");
     * REQUIRE(interval >= 0, "Interval can't be negative");
     * REQUIRE(transport >= 0, "Transport can't be negative");
     * REQUIRE(renewal >= 0, "Renewal can't be negative");
     *
     * @post
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     */
    Vaccin(std::string& type, const int delivery, const int interval, const int transport, const int renewal,
           const int temp);

    /**
     * \brief Check whether the Hub object is properly initialised
     *
     * @return true when object is properly initialised, false when not
     */
    bool properlyInitialized() const;

    /**
     * \brief Get type
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Type as string
     */
    const std::string &getType() const;

    /**
     * \brief Get amount of vaccins
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of vaccins
     */
    int getDelivery() const;

    /**
     * \brief Get interval between deliveries
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Interval between deliveries as int
     */
    int getInterval() const;

    /**
     * \brief Get renewal between Vaccin shots
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Renewal as int
     */
    int getRenewal() const;

    /**
     * \brief Get temperature of Vaccin to be stored
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Temperature as int
     */
    int getTemperature() const;

    /**
     * \brief Get amount of vaccins delivered to VaccinationCenters
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of vaccins delivered to VaccinationCenters as int
     */
    int getTransport() const;

    /**
     * \brief Get amount of vaccins currently of this type
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of vaccins as int
     */
    int getVaccin() const;

    /**
     * \brief Update the amount of vaccins
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @post
     *
     */
    void updateVaccins();

    /**
     * \brief subtracts transportAmount from fvaccin
     *
     * @param transportAmount Amount of Vaccins that will be transported to a VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @post
     *
     */
    void updateVaccinsTransport(const int transportAmount);

    /**
     * \brief Calculate how many cargo's there will be transported to a Center
     *
     * @param center Pointer to the VaccinationCenter which transport shall be calculated
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @post
     * ENSURE(vaccinsTransport <= this->fvaccin, "Amount of vaccinsTransport is too high")
     * ENSURE((vaccinsTransport <= (2 * center->getCapacity())), "Amount of vaccinsTransport is too high")
     *
     * @return Amount of cargo that will be transported as int
     */
     int calculateTransport(const VaccinationCenter * center, bool zeroVaccin) const;

    /**
     * \brief Print out data of Vaccin
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     */
    void print(std::ofstream& stream) const;
};

#endif //VACCINDISTRIBUTOR_VACCIN_H
