/**
 * @file VaccinationCenter.h
 * @brief This header file contains the declarations and the members of the VaccinationCenter class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#ifndef TTT_VACCINATIONCENTER_H
#define TTT_VACCINATIONCENTER_H

#include <string>
#include <iostream>
#include <fstream>
#include "DesignByContract.h"
#include "Utils.h"

/**
 * \brief Class implemented for a VaccinationCenter
 */
class VaccinationCenter {

private:
    std::string fname; ///< Name of the VaccinationCenter
    std::string faddress; ///< Address of the VaccinationCenter
    int fpopulation; ///< Amount of people the VaccinationCenter is responsible for
    int fcapacity; ///< Amount of vaccins that can be stored
    int fvaccins; ///< Amount of vaccins currently in the VaccinationCenter
    int fvaccinated; ///< Amount of people already vaccinated
    VaccinationCenter *_initCheck;

public:

    /**
     * \brief Non-default constructor for a VaccinationCenter object
     *
     * @param name Name of the VaccinationCenter
     * @param address Address of the VaccinationCenter
     * @param population Amount of people the Vaccination is responsible for
     * @param capacity Amount of Vaccins that can be stored
     *
     * @pre
     * REQUIRE(fname.length() > 0, "Name can't be empty");
     * REQUIRE(fname.length() > 0, "Adres can't be empty");
     * REQUIRE(fpopulation >= 0, "Negative population'");
     * REQUIRE(fcapacity >= 0, "Negative capacity");
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    VaccinationCenter(const std::string &name, const std::string &address, int population, int capacity);

    /**
    * \brief Check whether the VaccinationCenter object is properly initialised
    *
    * @return true when object is properly initialised, false when not
    */
    bool properlyInitialized() const;

    /**
     * \brief Get name of the VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Name of VaccinationCenter as string
     */
    const std::string &getName() const;

    /**
     * \brief Get address of the VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Address of VaccinationCenter as string
     */
    const std::string &getAddress() const;

    /**
     * \brief Get amount of people the VaccinationCenter is responsible for
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Population as int
     */
    int getPopulation() const;

    /**
     * \brief Get capacity of the VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Capacity of VaccinationCenter as int
     */
    int getCapacity() const;

    /**
     * \brief Get amount of vaccins of the VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Amount of vaccins of the VaccinationCenter as int
     */
    int getVaccins() const;

    /**
     * \brief Get amount of people vaccinated of the VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Amount of people vaccinated as int
     */
    int getVaccinated() const;

    /**
     * \brief Add amount of vaccins to Center
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @post
     * ENSURE(fvaccins <= fcapacity * 2, "Amount of vaccins must not exceed capacity of Center")
     *
     */
    void addVaccins(int amount);

    /**
     * \brief Amount of peaple that can be vaccineded/day
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     */
    int calculateVaccinationAmount();

    /**
     * \brief Vacinate center, update fvaccins and fvaccins
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(fvaccins <= fcapacity * 2, "Amount of vaccins must not exceed capacity");
     *
     * @param stream Output stream
     *
     */
    void vaccinateCenter(std::ostream &stream);

    /**
     * \brief Print out data of VaccinationCenter
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     */
    void print(std::ofstream &stream) const;

    /**
     *\brief Prints out data of VaccinationCenter the fancy way with a progressbar
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     */
    void printGraphical(std::ostream &stream) const;
};

#endif //TTT_VACCINATIONCENTER_H
