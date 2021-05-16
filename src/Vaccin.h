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
protected:
    std::string ftype; ///< Type of the vaccin
    int fvaccinTemperature; ///< Temperature required to store the Vaccin
    int fvaccinAmount; ///< Amount of vaccins from this type currently in the hub
    int fvaccinRenewal; ///< Interval between two shots of the Vaccin

};

/**
 * \brief Class implemented for Vaccin object
 */
class VaccinInHub: Vaccin {
private:
    int fdelivery; ///< Amount of vaccins delivered to Hub on the start and each interval
    int finterval; ///< Time between deliveries
    int ftransport; ///< Amount of vaccins from this type delivered to other VaccinationCenters
    int fdelivered; ///< Total delivered amount of vaccins
    VaccinInHub* _initCheck;

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
    VaccinInHub(std::string type, const int delivery, const int interval, const int transport, const int renewal,
           const int temp);

    /**
     * @brief Default constructor for Vaccin type object
     *
     * @post
     * ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state")
     */
    VaccinInHub() {
        this->_initCheck = this;
        ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
    }

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
     */
    void copyVaccin(const VaccinInHub *v);

    /**
     * \brief Check whether the Vaccin object is properly initialised
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

class VaccinInCenter: Vaccin{
    /**
 * \brief struct implemented to hold every type of Vaccin that the Center has stored
 */
private:
    std::map<int, int> ftracker; ///< <Days till second shot, amount of people with first shot>
    VaccinInCenter* _initCheck;

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
     */
    VaccinInCenter(const std::string &vaccinType, int vaccinTemperature, int vaccinRenewal, int vaccinAmount) {
        this->ftype = vaccinType;
        this->fvaccinTemperature = vaccinTemperature;
        this->fvaccinRenewal = vaccinRenewal;
        this->fvaccinAmount = vaccinAmount;
        this->_initCheck = this;
        ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
    }

    /**
     * \brief Constructor for vaccinType object
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    VaccinInCenter() {
        this->_initCheck = this;
        ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
    }

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
     *
     */
    void copyVaccin(const VaccinInCenter *v) {

        REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
        REQUIRE(v->ftype != "", "Vaccin type must not be empty");
        REQUIRE(v->fvaccinAmount >= 0, "Vaccin must not be negative");
        REQUIRE(v->fvaccinRenewal >= 0, "Vaccin renewal must not be negative");

        this->ftype = v->getVaccinType();
        this->fvaccinTemperature = v->getVaccinTemperature();
        this->fvaccinAmount = v->getVaccinAmount();
        this->fvaccinRenewal = v->getVaccinRenewal();
        this->ftracker = v->getTracker();

        this->_initCheck = this;
        ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state");
    }

    /**
    * \brief Check whether the VaccinationCenter object is properly initialised
    *
    * @pre
    * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
    *
    * @return true when object is properly initialised, false when not
    */
    bool properlyInitialized() const {

        return this == this->_initCheck;
    }

    /**
     * \brief Get vaccinType name
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Name as string
     */
    const std::string &getVaccinType() const {

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        return ftype;
    }

    /**
     * \brief Get vaccinType temp
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Temperature as int
     */
    int getVaccinTemperature() const {

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        return fvaccinTemperature;
    }

    /**
     * \brief Get vaccinType renewal time
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Renewal as int
     */
    int getVaccinRenewal() const {

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        return fvaccinRenewal;
    }

    /**
     * \brief Get vaccinType amount
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of Vaccins as int
     */
    int &getVaccinAmount() {

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        return fvaccinAmount;
    }

    /**
     * \brief Get vaccinType amount const
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of Vaccins as int
     */
    int getVaccinAmount() const{

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        return fvaccinAmount;
    }

    /**
     * \brief Get vaccinType tracker (const)
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return <days left of renewal, amount vaccinated> as std::map<int, int>
     */
    const std::map<int, int> &getTracker() const {

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        return ftracker;
    }

    /**
     * \brief Get vaccinType tracker
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return <days left of renewal, amount vaccinated> as std::map<int, int>
     */
    std::map<int, int> &getTracker() {

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        return ftracker;
    }

    /**
     * \brief Update every renewal in tracker and merge if zero
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     */
    void addDay(){

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        std::map<int, int> newTracker;
        newTracker[0] = 0;
        for (std::map<int, int>::iterator it = ftracker.begin(); it != ftracker.end(); it++){
            if(it->first + 1 <= 0) {
                newTracker[it->first + 1] = it->second;
            }
            else{
                newTracker[0] += it->second;
            }
        }
        ftracker = newTracker;
    }

    /**
     * \brief Insert at day amount of people vaccinated
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @param day Day left of renewal to be inserted in map
     * @param requiredPeople Amount of vaccinated people to add or remove
     */
    void insertRequiredDay(int day, int requiredPeople){
        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        if(requiredPeople > 0){
            ftracker[day] += requiredPeople;
        }
    }

    /**
     * \brief Check if this Vaccin is of type renewal
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return true if renewal
     */
    bool isRenewal() const {

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        return this->fvaccinRenewal != 0;
    }

    /**
     * \brief Gives total of people who got first Vaccin shot
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Vaccin must be properly initialized")
     *
     * @return Amount of people with first shot of Vaccin as int
     */
    int totalFirstVaccination() const {

        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");

        int total = 0;
        for (std::map<int, int>::const_iterator it = ftracker.begin(); it != ftracker.end(); it++){
            total += it->second;
        }
        return total;
    }

    void removeVaccin(){
        ENSURE(properlyInitialized(), "Vaccin must be properly initialized");
        this->fvaccinAmount = 0;

    }
};

#endif //VACCINDISTRIBUTOR_VACCIN_H
