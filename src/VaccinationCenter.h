/**
 * @file VaccinationCenter.h
 * @brief This header file contains the declarations and the members of the VaccinationCenter class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#ifndef TTT_VACCINATIONCENTER_H
#define TTT_VACCINATIONCENTER_H

#include <string>
#include <iostream>
#include <fstream>
#include "DesignByContract.h"

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
     * \brief Default constructor for a VaccinationCenter object
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    VaccinationCenter();

    /**
     * \brief Non-default constructor for a VaccinationCenter object
     *
     * @param fname Name of the VaccinationCenter
     * @param faddress Address of the VaccinationCenter
     * @param fpopulation Amount of people the Vaccination is responsible for
     * @param fcapacity Amount of Vaccins that can be stored
     *
     * @pre
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    VaccinationCenter(const std::string &fname, const std::string &faddress, int fpopulation, int fcapacity);

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
};

#endif //TTT_VACCINATIONCENTER_H
