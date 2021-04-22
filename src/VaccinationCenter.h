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
class Vaccin;

/**
 * \brief Class implemented for a VaccinationCenter
 */
class VaccinationCenter {

/**
 * \brief struct implemented to hold every type of Vaccin that the Center has stored
 */
struct vaccinType {

public:
    /**
     * \brief Constructor
     *
     * @param vaccinType Name / type of the Vaccin as string
     * @param vaccinTemperature Temperature of Vaccin as int
     * @param vaccinRenewal Renewal time of Vaccin as int
     * @param vaccinAmount Amount of vaccins stored as int
     */
    vaccinType(const std::string &vaccinType, int vaccinTemperature, int vaccinRenewal, int vaccinAmount)
            : fvaccinType(vaccinType), fvaccinTemperature(vaccinTemperature), fvaccinRenewal(vaccinRenewal),
              fvaccinAmount(vaccinAmount) {}

    /**
     * \brief Get vaccinType name
     *
     * @return Name as string
     */
    const std::string &getVaccinType() const {
        return fvaccinType;
    }

    /**
     * \brief Get vaccinType temp
     *
     * @return Temperature as int
     */
    int getVaccinTemperature() const {
        return fvaccinTemperature;
    }

    /**
     * \brief Get vaccinType renewal time
     *
     * @return Renewal as int
     */
    int getVaccinRenewal() const {
        return fvaccinRenewal;
    }

    /**
     * \brief Get vaccinType amount
     *
     * @return Amount of Vaccins as int
     */
    int &getVaccinAmount() {
        return fvaccinAmount;
    }

    /**
     * \brief Get vaccinType amount const
     *
     * @return Amount of Vaccins as int
     */
    int getVaccinAmount() const{
        return fvaccinAmount;
    }

    /**
     * \brief Get vaccinType tracker
     *
     * @return <days left of renewal, amount vaccinated> as std::map<int, int>
     */
    std::map<int, int> &getTracker() {
        return ftracker;
    }

    /**
     * \brief Update every renewal in tracker and merge if zero
     */
    void addDay(){

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
     * @param day Day left of renewal to be inserted in map
     * @param requiredPeople Amount of vaccinated people to add or remove
     */
    void insertRequiredDay(int day, int requiredPeople){

        ftracker[day] += requiredPeople;
    }

    /**
     * \brief Check if this Vaccin is of type renewal
     *
     * @return true if renewal
     */
    bool isRenewal() const {

        return this->fvaccinRenewal != 0;
    }

    /**
     * \brief Gives total of people who got first Vaccin shot
     *
     * @return Amount of people with first shot of Vaccin as int
     */
    int totalFirstVaccination() const {

        int total = 0;
        for (std::map<int, int>::const_iterator it = ftracker.begin(); it != ftracker.end(); it++){
            total += it->second;
        }
        return total;
    }

private:
    std::string fvaccinType; ///< Type of the vaccin
    int fvaccinTemperature; ///< Temperature required to store the Vaccin
    int fvaccinRenewal; ///< Interval between two shots of the Vaccin
    int fvaccinAmount; ///< Amount of vaccins of this type
    std::map<int, int> ftracker; ///< <Days till second shot, amount of people with first shot>
};

private:
    std::string fname; ///< Name of the VaccinationCenter
    std::string faddress; ///< Address of the VaccinationCenter
    int fpopulation; ///< Amount of people the VaccinationCenter is responsible for
    int fcapacity; ///< Amount of peaple that can be vaccined on one day
    //int fvaccins;  ///< Amount of vaccins currently in the VaccinationCenter
    int fvaccinated; ///< Amount of people already vaccinated
    std::map<const std::string, vaccinType*> fvaccinsType; ///< Map with name of vaccin type and pointer to vaccinType
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
     * ENSURE(this->getVaccins() <= fcapacity * 2, "Amount of vaccins must not exceed capacity of Center")
     */
    void addVaccins(const int amount, const Vaccin* vaccin);

    /**
     * \brief Amount of people that can be vaccinated that day
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Amount of people that can be vaccinated as int
     */
    int calculateVaccinationAmount();

    /**
     * \brief Amount of people that can be vaccinated that day considering renewal period of a Vaccin
     *
     * @param vaccin Pointer to vaccinType object
     * @param alreadyVaccinated Amount of people already vaccinated with a first shot of the Vaccin
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     *
     * @return Amount of people that can be vaccinated as int
     */
    int calculateVaccinationAmount(const VaccinationCenter::vaccinType* vaccin, int alreadyVaccinated) const;

    /**
    * \brief Calculate amount of people that need 2nd vaccination from specified Vaccin and will check if more people
    *        can be vaccinated if there are Vaccins left of the type
    *
    * @param vaccin Pointer to the vaccinType object
    * @param alreadyVaccinated Amount of people that already got vaccinated with a first shot of the Vaccin
     *
    * @pre
    * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
    *
    * @return Amount of people that can be vaccinated as int with a 2nd shot of the Vaccin with renewal as int
    */
    int calculateVaccinationAmountRenewal(VaccinationCenter::vaccinType* vaccin, const int alreadyVaccinated);

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
    std::map<const std::string, vaccinType*> getVaccin(bool zeroVaccin) const;

    /**
     * \brief Vaccinate center and update fvaccins
     *
     * @param stream Output stream
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(this->getVaccins() <= fcapacity * 2, "Amount of vaccins must not exceed capacity")
     */
    void vaccinateCenter(std::ostream &stream);

    /**
     * \brief Vaccinate all vaccins in a given map
     *
     * @param vaccinsType Map of vaccins with name of vaccinType and pointer to vaccinType object
     * @param stream Output stream
     * @param alreadyVaccinatedTodau Amount of people already vaccinated today
     *
     * @pre
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(fvaccins <= fcapacity * 2, "Amount of vaccins must not exceed capacity");
     */
    int vaccinateCenter(std::map<const std::string, vaccinType*> vaccinsType, std::ostream &stream,  int alreadyVaccinatedTodau);

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
    int getOpenVaccinStorage(Vaccin* vaccin);

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
    int requiredAmountVaccin(VaccinationCenter::vaccinType *vaccin);

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
     * \brief Deconstructor for VaccinationCenter object
     */
    ~VaccinationCenter();
};

#endif //TTT_VACCINATIONCENTER_H
