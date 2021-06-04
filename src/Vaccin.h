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

class Vaccin{
public:
    /**
     * \brief Check whether the VaccinationCenter object is properly initialised
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
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
     * \brief Get temperature of Vaccin to be stored
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Temperature as int
     */
    int getTemperature() const;

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
     * \brief Get amount of vaccins currently of this type
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of vaccins as int
     */
    int getVaccin() const;

protected:
    std::string ftype; ///< Type of the vaccin
    int fvaccinTemperature; ///< Temperature required to store the Vaccin
    int fvaccinAmount; ///< Amount of vaccins from this type currently in the hub
    int fvaccinRenewal; ///< Interval between two shots of the Vaccin
    Vaccin* _initCheck;
};

/**
 * \brief Class implemented for Vaccin object
 */
class VaccinInHub:public Vaccin {
private:
    int fdelivery; ///< Amount of vaccins delivered to Hub on the start and each interval
    int finterval; ///< Time between deliveries
    int ftransport; ///< Amount of vaccins from this type delivered to other VaccinationCenters
    int fdelivered; ///< Total delivered amount of vaccins

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
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
     * ENSURE(this->ftype == type, "type is not set to value");
     * ENSURE(this->fdelivery == delivery, "delivery is not set to value");
     * ENSURE(this->finterval == interval, "interval is not set to value");
     * ENSURE(this->ftransport == transport, "transport is not set to value");
     * ENSURE(this->fvaccinRenewal == renewal, "vaccinRenewal is not set to value");
     * ENSURE(this->fvaccinTemperature == temp, "vaccinTemperature is not set to value");
     * ENSURE(this->fvaccinAmount == delivery, "vaccinAmount is the same value as delivery");
     * ENSURE(this->fdelivered == 0, "delivered must be 0");
     */
    VaccinInHub(std::string type, const int delivery, const int interval, const int transport, const int renewal,
           const int temp);

    /**
     * @brief Default constructor for Vaccin type object
     *
     * @post
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     */
    VaccinInHub();

    /**
     * @\brief Copy constructor for Vaccin object
     *
     * @param v Vaccin object to be copied from
     *
     * @pre
     * REQUIRE(v->properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(v->getType() != "", "Type must not be negative")
     * REQUIRE(v->getDelivery() >= 0, "Delivery must not be negative")
     * REQUIRE(v->getInterval() >= 0, "Interval must not be negative")
     * REQUIRE(v->getTransport() >= 0, "Transport must not be negative")
     * REQUIRE(v->getRenewal() >= 0, "Renewal must not be negative")
     * REQUIRE(v->getVaccin() >= 0, "Amount of vaccins must not be negative")
     *
     * @post
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     * ENSURE(this->ftype == v->getType(), "type must be the same!");
     * ENSURE(this->fdelivery == v->getDelivery(), "delivery must be the same");
     * ENSURE(this->finterval == v->getInterval(), "interval must be the same");
     * ENSURE(this->ftransport == v->getTransport(), "transport must be the same");
     * ENSURE(this->fvaccinRenewal == v->getRenewal(), "vaccinRenewal must be the same");
     * ENSURE(this->fvaccinTemperature == v->getTemperature(), "vaccinTemperature must be the same");
     * ENSURE(this->fvaccinAmount == v->getVaccin(), "vaccinAmount must be the same");
     * ENSURE(this->fdelivered == v->getDelivered(), "delivered must be the same");
     */
    void copyVaccin(const VaccinInHub *v);

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
     * \brief Set amount of vaccins delivered
     *
     * @param newDelivery New amount of vaccins delivered as int
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(newDelivery >= 0, "Delivery must not be negative")
     */
    void setDelivery(const int &newDelivery);

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
     * \brief Set interval between deliveries
     *
     * @param newInterval New interval between deliveries as int
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(newInterval >= 0, "newInterval must not be negative")
     */
    void setInterval(const int &newInterval);

    /**
     * \brief Set renewal between Vaccin shots
     *
     * @param newRenewal new Renewal time as int
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(newRenewal >= 0, "newRenewal must not be negative")
     */
    void setRenewal(const int &newRenewal);

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
     * \brief Set amount of vaccins delivered to VaccinationCenters
     *
     * @param newTransport Amount of vaccins delivered to Center as int
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(newTransport >= 0, "newTransport must not be negative")
     */
    void setTransport(const int &newTransport);

    /**
     * \brief Get total amount of delivered vaccins
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of delivered vaccins as int
     */
    int getDelivered() const;

    /**
     * \brief Update the amount of vaccins with delivery amount
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @post
     * ENSURE(this->getVaccin() >= this->getDelivery(), "The amount of vaccins must be bigger delivery amount (fvaccin += fdelivery)");
     */
    void updateVaccins();

    /**
     * \brief subtracts transportAmount from fvaccins
     *
     * @param transportAmount Amount of Vaccins that will be transported to a VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(transportAmount % this->getTransport() == 0, "Wrong transport amount, Cargo amount must be a int")
     *
     * @post
     * ENSURE(this->getVaccin() > 0, "fvaccinAmount can not be negative (fvaccinAmount -= transportAmount)");
     * ENSURE(this->getDelivered() >= transportAmount, "fdelivered must be lager then transportAmount (this->getDelivered() >= transportAmount)");
     */
    void updateVaccinsTransport(const int transportAmount);

    /**
     * \brief Check if Vaccin must be stored under 0 degrees
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return bool:
     *              true -> (temp < 0)
     *              false -> (temp >= 0)
     */
    bool checkUnderZero();
};

class VaccinInCenter: public Vaccin{
    /**
 * \brief struct implemented to hold every type of Vaccin that the Center has stored
 */
private:
    std::map<int, int> ftracker; ///< <Days till second shot, amount of people with first shot>
public:
    /**
     * \brief Constructor for vaccinType object
     *
     * @param vaccinType Name / type of the Vaccin as string
     * @param vaccinTemperature Temperature of Vaccin as int
     * @param vaccinRenewal Renewal time of Vaccin as int
     * @param vaccinAmount Amount of vaccins stored as int
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     * ENSURE(this->ftype == vaccinType, "type is not set to given value");
     * ENSURE(this->fvaccinRenewal == vaccinRenewal, "vaccinRenewal is not set to given value");
     * ENSURE(this->fvaccinTemperature == vaccinTemperature, "vaccinTemperature is not set to given value");
     * ENSURE(this->fvaccinAmount == vaccinAmount, "vaccinAmount is not set to given value");
     */
    VaccinInCenter(const std::string &vaccinType, int vaccinTemperature, int vaccinRenewal, int vaccinAmount);

    /**
     * \brief Constructor for vaccinType object
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    VaccinInCenter();

    /**
     * @brief Copy constructor for VaccinType object
     *
     * @param v VaccinType object to be copied from
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(v->fvaccinType != "", "Vaccin type must not be empty")
     * REQUIRE(v->fvaccinAmount >= 0, "Vaccin must not be negative")
     * REQUIRE(v->fvaccinRenewal >= 0, "Vaccin renewal must not be negative")
     *
     * @post
     * ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state")
     * ENSURE(this->ftype == v->getType(), "type must be the same!");
     * ENSURE(this->fvaccinRenewal == v->getRenewal(), "vaccinRenewal must be the same");
     * ENSURE(this->fvaccinTemperature == v->getTemperature(), "vaccinTemperature must be the same");
     * ENSURE(this->fvaccinAmount == v->getVaccin(), "vaccinAmount must be the same");
     *
     */
    void copyVaccin(const VaccinInCenter *v);

    /**
     * \brief Get vaccinType amount
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of Vaccins as int
     */
    int &getVaccinAmount();

    /**
     * \brief Get vaccinType tracker (const)
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return <days left of renewal, amount vaccinated> as std::map<int, int>
     */
    const std::map<int, int> &getTracker() const;

    /**
     * \brief Get vaccinType tracker
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return <days left of renewal, amount vaccinated> as std::map<int, int>
     */
    std::map<int, int> &getTracker();

    /**
     * \brief Update every renewal in tracker and merge if zero
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     */
    void addDay();

    /**
     * \brief Insert at day amount of people vaccinated
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     * REQUIRE(requiredPeople >= 0, "Amount of peaple can not be negative");
     *
     * @post
     * ENSURE(requiredPeople == 0 || ftracker.find(day) != ftracker.end(),"Day not added");
     *
     * @param day Day left of renewal to be inserted in map
     * @param requiredPeople Amount of vaccinated people to add or remove
     */
    void insertRequiredDay(int day, int requiredPeople);

    /**
     * \brief Check if this Vaccin is of type renewal
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return true if renewal
     */
    bool isRenewal() const;

    /**
     * \brief Gives total of people who got first Vaccin shot
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of people with first shot of Vaccin as int
     */
    int totalFirstVaccination() const;

    /**
     * \brief Set vaccinamount to 0
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @post
     * ENSURE(this->getVaccinAmount() == 0, "fvaccinAmount is not set to 0");
     */
    void removeVaccin();
};

#endif //VACCINDISTRIBUTOR_VACCIN_H
