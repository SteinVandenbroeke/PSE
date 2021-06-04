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
#include <cmath>

/**
 * \brief Class implemented for a Hub
 */
class Hub {

private:
    std::map<std::string, VaccinInHub*> fvaccins; ///< Map with pointers to Vaccins of hub
    std::map<std::string, VaccinationCenter*> fcentra ; ///< Map with the connected VaccinationCenters
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
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     *
     * @post
     * ENSURE((double)(center->getCapacity())/(double)(this->totalVaccinCentraCapacity()) <= 1, "Ratio cannot be larger than 1");
     */
    Hub();

    /**
     * @brief copyHub Copy constructor for Hub object
     *
     * @param h Hub object to be copied from
     *
     * @param centra Map with all existing centra's in Simulation
     *
     * @pre
     * REQUIRE(h->properlyInitialized(), "Constructor must end in properlyInitialized state")
     *
     * @post
     * ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state")
     */
    void copyHub(const Hub *h, const std::map<std::string, VaccinationCenter*> &centra);

    /**
     * \brief Deconstructor for a Hub object
     */
    ~Hub();

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
    const std::map<std::string, VaccinInHub*> &getVaccins() const;

    /**
     * \brief Get vaccins of Hub object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Map with name of Vaccin as key and pointer to Vaccin object as value
     */
    std::map<std::string, VaccinInHub*> &getVaccins();

    /**
     * \brief Add vaccin to Hub object
     *
     * @param Vaccin Pointer to Vaccin object to add
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized");
     * REQUIRE(vaccin->properlyInitialized(), "Vaccin must be properly initialized");
     * REQUIRE(!containsVaccin(vaccin), "Vaccin can't yet exist in Hub");
     *
     * @post
     * ENSURE(containsVaccin(vaccin), "Vaccin must be added to Hub");
     */
    void addVaccin(VaccinInHub* Vaccin);

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
     * \brief Add VaccinationCenter to map with connections of Hub
     *
     * @param name Name of the VaccinationCenter
     * @param center Pointer to VaccinationCenter object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized");
     * REQUIRE(center->properlyInitialized(), "VaccinationCenter must be properly initialized");
     * REQUIRE(!this->containsVaccinationCenter(name) ,"VaccinationCenter must not exist yet in map");
     *
     * @post
     * ENSURE(this->containsVaccinationCenter(name),"VaccinationCenter must exist in map");
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
    * \brief Distribute vaccin fairly between the centers and over the days (without considering 2nd vaccination)
    *
    * @param vaccin: Vaccin that needs to be distributed
    * @param currentDay: the current day of the simulation
    * @param stream: stream Output stream
    *
    * @pre
    * REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    * REQUIRE(currentDay >= 0, "currentDay cannot be negative");
    * REQUIRE(vaccin->properlyInitialized(), "VaccinationCenter must be properly initialized");
    * REQUIRE(containsVaccin(vaccin), "Given vaccin must exist");
    *
    */
    void distributeVaccinsFair(VaccinInHub* vaccin, int currentDay, std::ostream &stream);

    /**
    * \brief Distribute required vaccins over an vaccinationCenter
    *
    * @param vaccinationCenter: gives this center the required vaccins
    * @param stream: stream Output stream
    *
    * @pre
    * REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    * REQUIRE(vaccinationCenter->properlyInitialized(), "VaccinationCenter must be properly initialized");
    * REQUIRE(vaccinationCenter->properlyInitialized(), "VaccinationCenter must be properly initialized");
    *
    */
    void distributeRequiredVaccins(VaccinationCenter* vaccinationCenter, std::ostream &stream);

    /**
    * \brief Gives the most suitable VaccinationCenter for a specified amount of a vaccin type
    *
    * @param vaccinCount: amount of Vaccins
    * @param vaccin: vaccin type
    *
    * @pre
    * REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    * REQUIRE(vaccin->properlyInitialized(), "Vaccin must be properly initialized");
    * REQUIRE(vaccinCount >= 0, "vaccinCount cannot be negative");
    * REQUIRE(getVaccins().find(vaccin->getType()) == getVaccins().end(), "Vaccin can't yet exist in Hub");
    */
    VaccinationCenter* mostSuitableVaccinationCenter(int vaccinCount, VaccinInHub* vaccin);

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

    /**
     * \brief Hold the return data of the stockToSize() function
     */
    struct stockToSizeReturn {
        /**
         * \brief Constructor for stockToSizeReturn object
         * @param cubeScale Scale of cube as double
         * @param coneScale Scale of cone as double
         * @param coneN Amount of corner points used in the ground surface of cone as int
         * @param coneCenterZ z-value of cartesian coordinates of cone as double
         */
        stockToSizeReturn(double cubeScale, double coneScale, double coneN, double coneCenterZ) : cubeScale(cubeScale),
                                                                                                  coneScale(coneScale),
                                                                                                  coneN(coneN),
                                                                                                  coneCenterZ(
                                                                                                          coneCenterZ) {}
        double cubeScale; ///< Scale of cube as double
        double coneScale; ///< Scale of cone as double
        int coneN; ///< Amount of corner points used in the ground surface of cone as int
        double coneCenterZ; ///< z-value of cartesian coordinates of cone as double
    };

    /**
     * \brief Calculate the properties of the Hub in a 3D-environment depending on the number
     *        of Vaccins in the Hub versus the total number of Vaccins that can be delivered
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return stockToSize object with data to construct the Hub in a 3D-environment
     */
    const stockToSizeReturn stockToSize() const;

    /**
     * \brief Generate a .ini file that describes the Hub in a 3D-environment
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @post
     * ENSURE(x != "", "ini file can not be empty!");
     *
     * @param stream Output stream
     * @param counterFigures Current number of figures already in the .ini file
     * @param counterHub Current number of Hubs already in the .ini file
     *
     * @return Cartesian coordinates of Hub as string in .ini file format
     */
    std::string generateIni(std::ofstream & stream, int & counterFigures, int & counterHub) const;

    /**
     * \brief Check if centerName exist in Hub vaccinnationcenter map
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @param name Name of the VaccinationCenter
     *
     * @return bool: true = exists | false = does not exits
     */
    bool containsVaccinationCenter(const std::string &name) const;

    /**
     * \brief Check if Vaccin exist in Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @param Vaccin pointer
     *
     * @return bool: true = exists | false = does not exits
     */
    bool containsVaccin(const VaccinInHub*);
};

#endif //TTT_HUB_H
