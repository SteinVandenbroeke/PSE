/**
 * @file VaccinationCenter.h
 * @brief This header file contains the declarations and the members of the VaccinationCenter class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#ifndef TTT_VACCINATIONCENTER_H
#define TTT_VACCINATIONCENTER_H

#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include "DesignByContract.h"
#include "Utils.h"

/**
 * \brief Forward declaration of the Vaccin class
 */
class VaccinInHub;
class VaccinInCenter;

/**
 * \brief Class implemented for a VaccinationCenter
 */
class VaccinationCenter {
private:
    std::string fname; ///< Name of the VaccinationCenter
    std::string faddress; ///< Address of the VaccinationCenter
    int fpopulation; ///< Amount of people the VaccinationCenter is responsible for
    int fcapacity; ///< Amount of peaple that can be vaccined on one day
    int fvaccinated; ///< Amount of people already vaccinated
    std::map<const std::string, VaccinInCenter*> fvaccinsType; ///< Map with name of vaccin type and pointer to vaccinType
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
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
     * ENSURE(this->getName() == fname, "Value of fname not set");
     * ENSURE(this->getAddress() == faddress, "Value of faddress not set");
     * ENSURE(this->getPopulation() == fpopulation, "Value of fpopulation not set");
     * ENSURE(this->getCapacity() == fcapacity, "Value of fcapacity not set");
     * ENSURE(this->getVaccinated() == 0, "fvaccinated is not 0");
     */
    VaccinationCenter(const std::string &name, const std::string &address, int population, int capacity);

    /**
     * \brief Default constructor for a VaccinationCenter object
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    VaccinationCenter();

    /**
     * @brief Copy constructor for a VaccinationCenter object
     *
     * @param v VaccinationCenter object top be copied from
     *
     * @pre
     * REQUIRE(v->properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(v->getName().length() > 0, "Name can't be empty")
     * REQUIRE(v->getAddress().length() > 0, "Adres can't be empty")
     * REQUIRE(v->getPopulation() >= 0, "Negative population")
     * REQUIRE(v->getCapacity() >= 0, "Negative capacity")
     * REQUIRE(v->getVaccinated() >= 0, "Negative vaccinated")
     * REQUIRE(v->getVaccins() >=0, "Negative vaccins")
     *
     * @post
     * ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state")
     * ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state");
     * ENSURE(this->getName() == v->getName(), "Name is not the same");
     * ENSURE(this->getAddress() == v->getAddress(), "Adress is not the same");
     * ENSURE(this->getPopulation() == v->getPopulation(), "Population is not the same");
     * ENSURE(this->getCapacity() == v->getCapacity(), "Capacity is not the same");
     * ENSURE(this->getVaccinated() == v->getVaccinated(), "Vaccinated is not the same");
     */
    void copyVaccinationCenter( const VaccinationCenter *v);

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
     * \brief Set capacity of VaccinationCenter
     *
     * @param newCapacity New capacity value as int
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(v->getCapacity() >= 0, "Negative capacity")
     */
    void setCapacity(const int &newCapacity);

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
     * \brief Get amount of vaccins of the VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Amount of vaccins of the VaccinationCenter as int
     */
    bool checkAmountVaccins() const;

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
     * \brief Add Vaccin to VaccinationCenter
     *
     * @param amount Amount of vaccins to be added
     * @param vaccin Pointer to Vaccin Object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(amount + this->getVaccins() <= (this->getCapacity() * 2), "Amount of vaccins must not exceed capacity of Center")
     *
     * @post
     * ENSURE(checkAmountVaccins(), "Amount of vaccins must not exceed capacity of Center");
     * ENSURE(vaccinsType().find(vaccin->getType())->second->getVaccinAmount() >= amount, "Amount of vaccins must be bigger then the added amount (+= amount)");
     */
    void addVaccins(const int amount, const VaccinInHub* vaccin);


    /**
     * \brief  gives map with name of vaccin type and pointer to vaccinType
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return map with name of vaccin type and pointer to vaccinType
     */
    const std::map<const std::string, VaccinInCenter*> vaccinsType() const;

    /**
     * \brief  gives amount of people that need an vaccin
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return amount of people that need an vaccin
     */
    int calculateVaccinationAmount() const;

    /**
     * \brief Amount of people that can be vaccinated that day considering renewal period of a Vaccin
     *
     * @param vaccin Pointer to vaccinType object
     * @param vaccinsUsed Amount of vaccins used already in center
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Amount of people that can be vaccinated as int
     */
    int calculateVaccinationAmount(const VaccinInCenter* vaccin, int vaccinsUsed) const;


    /**
     * \brief Calculate amount of people that need a 2nd shot from the specified Vaccin, this function will also check
     *        if more people can be vaccinated using this Vaccin.
    *
    * @param vaccin Pointer to the vaccinType object
    * @param firstShot Amount of people that already received a first shot of the Vaccin
    * @param vaccinated Total amount of people vaccinated that day by the center
    * @param vaccinsUsed Total amount of vaccins used that day by the center
    *
    * @pre
    * REQUIRE(alreadyVaccinated >= 0, "Amount of people recieved first shot must not be negative")
    * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
    *
    * @return Amount of people that got a 2nd shot from the specified Vaccin as int
    */
    int calculateVaccinationAmountSecondShot(VaccinInCenter* vaccin, const int firstShot,
                                             int & vaccinated, int & vaccinsUsed);

    /**
    * \brief Function to update the days remaining for people who already got a first shot of a Vaccin with renewal
    *
    * @pre
    * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
    */
    void updateRenewal();

    /**
    * \brief Gives a map of all Vaccins or only the Vaccins with temp under zero
    *
    * @param zeroVaccin: true -> only underZero vaccins
    *
    * @pre
    * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
    *
    * @return map<name,vaccin>
    */
    std::map<const std::string, VaccinInCenter*> getVaccin(bool zeroVaccin) const;

    /**
     * \brief Vaccinate center and update fvaccins, will also delete any Vaccins that may not be needed and will cause
     *        problems for the Simulation
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(checkAmountVaccins(), "Amount of vaccins must not exceed capacity")
     *
     * @post
     * ENSURE(vaccinsUsed <= this->getCapacity(), "Amount of vaccinations must not exceed capacity")
     * ENSURE(vaccinsUsed <= this->getCapacity(), "Amount of vaccinations must not exceed capacity")
     * ENSURE(this->getVaccinated() <= this->getPopulation(), "Peaple that are vaccinated can not be more than the population")
     */
    void vaccinateCenter(std::ostream &stream);

    /**
     * \brief Vaccinate all vaccins in a given map
     *
     * @param vaccinsType Map of vaccins with name of vaccinType and pointer to vaccinType object
     * @param vaccinated Total amount of people vaccinated by the center that day
     * @param vaccinsUsed Total amount of vaccins used by the center that day
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(checkAmountVaccins(), "Amount of vaccins must not exceed capacity")
     *
     */
    void vaccinateCenter(std::map<const std::string, VaccinInCenter*> vaccinsType, int & vaccinated,
                                        int & vaccinsUsed);

    /**
     * \brief Print out data of VaccinationCenter
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     */
    void print(std::ostream &stream) const;

    /**
     * \brief Prints out data of VaccinationCenter the fancy way with a progressbar and overview of all the Vaccin types
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     */
    void printGraphical(std::ostream &stream) const;

    /**
     * \brief Gives the amount of vaccines needed for 2nd vaccination today
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return map<name vaccin, amount needed>
     */
    std::map<std::string, int> requiredAmountVaccinType();

    /**
     * \brief Gives the storage place for a specified Vaccin
     *
     * @param vaccin Pointer to Vaccin object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return How many Vaccins needed
     */
    int getOpenVaccinStorage(VaccinInHub* vaccin);

    /**
     * \brief Gives the amount of vaccines needed for 2nd vaccination of current day
     *
     * @param vaccin Pointer to vaccinType object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return amount needed
     */
    int requiredAmountVaccin(VaccinInCenter *vaccin);

    /**
     * \brief Calculate the scale of the VaccinationCenter in a 3D-environment depending on the number
     *        of Vaccins in the Center versus the total number of Vaccins that can be stored (=capacity)
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Scale of Center in 3D-environment as string in .ini file format
     */
    std::string stockToSize() const;

    /**
     * \brief Calculate color of VaccinationCenter in 3D-environment depending on the number of people already
     *        vaccinated versus the population that the Center operates in
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Color of Center in 3D-environment as string in .ini file format
     */
    std::string vaccinatedToColor() const;

    /**
     * \brief Generate a .ini file that describes the VaccinationCenter in a 3D-environment
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @param stream Output stream
     * @param counterFigures Current number of figures already in the .ini file
     * @param counterCenter Current number of Centra already in the .ini file
     * @param maxHubX x-value of cartesian coordinates of last Hub in 3D-environment
     *
     * @return x-value and y-value of the Cartesian coordinates of Center as pair of doubles
     */
    std::pair<double, double> generateIni(std::ofstream & stream, int & counterFigures, int & counterCenter,
                                          const double & maxHubX) const;

    /**
     * \brief Calculate total amount of people wainting for a second shot for all the Vaccins the Center stores
     *
     * @pre
     * ENSURE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Total amount of people waiting for a second shot for all Vaccins as int
     */

    int totalWaitingForSeccondPrik() const;

    /**
     * \brief Deconstructor for VaccinationCenter object
     *
     * @pre
     * ENSURE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     */
    ~VaccinationCenter();

};

#endif //TTT_VACCINATIONCENTER_H
