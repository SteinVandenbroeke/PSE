//
// Created by stein on 7/04/2021.
//

#ifndef VACCINDISTRIBUTOR_VACCIN_H
#define VACCINDISTRIBUTOR_VACCIN_H
#include "DesignByContract.h"
#include <string>


class Vaccin {

private:
    std::string ftype;
    int fdelivery; ///< Amount of vaccins delivered to Hub on the start and each interval
    int finterval; ///< Time between deliveries
    int ftransport; ///< Amount of vaccins from this type delivered to other VaccinationCenters
    int fvaccin; ///< Amount of vaccins from this type currently in the hub

    Vaccin *_initCheck;
public:

    /**
     * \brief Non-default constructor for a Hub object
     *
     * @param ftype name of vaccin
     * @param fdelivery Amount of vaccins that can de delivered to Hub
     * @param finterval Time between deliveries to Hub
     * @param ftransport Amount of vaccins in each transport to a VaccinationCenter
     *
     * @pre
     * REQUIRE(type != "", "type can't be empty");
     * REQUIRE(delivery >= 0, "Delivery can't be negative");
     * REQUIRE(interval >= 0, "Interval can't be negative");
     * REQUIRE(transport >= 0, "Transport can't be negative");
     *
     * @post
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     */
    Vaccin(std::string& type, int delivery, int interval, int transport);

    /**
     * \brief Check whether the Hub object is properly initialised
     *
     * @return true when object is properly initialised, false when not
     */
    bool properlyInitialized() const;

    /**
     * \brief Get amount of vaccins
     *
     * @pre
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     *
     * @return Amount of vaccins
     */
    int getDelivery() const;

    /**
     * \brief Get interval between deliveries
     *
     * @pre
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     *
     * @return Interval between deliveries as int
     */
    int getInterval() const;

    /**
     * \brief Get amount of vaccins delivered to VaccinationCenters
     *
     * @pre
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     *
     * @return Amount of vaccins delivered to VaccinationCenters as int
     */
    int getTransport() const;

    /**
     * \brief Get amount of vaccins currently of this type
     *
     * @pre
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     *
     * @return Amount of vaccins as int
     */
    int getVaccin() const;

    /**
     * \brief Update the amount of vaccins
     *
     * @pre
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     *
     */
    void updateVaccins();

    /**
     * \brief subtracts transportAmount from fvaccin
     *
     * @pre
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     *
     * @post
     *
     */
    void updateVaccinsTransport(int transportAmount)

};


#endif //VACCINDISTRIBUTOR_VACCIN_H
