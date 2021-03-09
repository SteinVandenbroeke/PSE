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

    const std::string &getName() const;

    const std::string &getAddress() const;

    int getPopulation() const;

    int getCapacity() const;

    int getVaccins() const;

    int getVaccinated() const;

    void print(std::ofstream &stream);
};


#endif //TTT_VACCINATIONCENTER_H
