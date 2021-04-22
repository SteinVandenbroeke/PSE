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
    std::map<std::string, Vaccin*> fvaccins; ///< Map with pointers to Vaccins of hub
    std::map<std::string, VaccinationCenter*> fcentra ; ///< Map with the connected VaccinationCenters
    Hub *_initCheck;

    /**
     * \brief Total vaccin capacity over all centers
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @post
     * ENSURE(totalCapacity > 0, "Total capacity can't be zero or negative");
     */
    int totalVaccinCentraCapacity();

    /**
     * \brief Gives the ratio between the capacity of the given centrum en the total capacity
     *
     * @param center: the ratio of this VaccinationCenter
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(center->properlyInitialized(), "VaccinationCenter must be properly initialized");
     *
     * @post
     *
     */
    double VaccinCentraCapacityRatio(VaccinationCenter* center);

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
     * \brief deconstructor for a Hub object
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

    int getTotalDelivery() const;

    /**
     * \brief Get vaccins of Hub object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Map with name of Vaccin as key and pointer to Vaccin object as value
     */
    const std::map<std::string, Vaccin *> &getVaccins() const;

    /**
     * \brief Get vaccins of Hub object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Map with name of Vaccin as key and pointer to Vaccin object as value
     */
    std::map<std::string, Vaccin *> &getVaccins();

    /**
     * \brief Add vaccin to Hub object
     *
     * @param Vaccin Pointer to Vaccin object to add
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(getVaccins().find(vaccin->getType()) == getVaccins().end(), "Vaccin can't yet exist in Hub")
     *
     * @post
     * ENSURE(getVaccins().find(vaccin->getType()) != getVaccins().end(), "Vaccin must be added to Hub")
     */
    void addVaccin(Vaccin* Vaccin);

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
     * \brief Get map with connected VaccinationCenters to Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     *
     * @return Map with connected VaccinationCenters to Hub
     */
    std::map<std::string, VaccinationCenter *> &getCentra();

    /**
     * \brief Add VaccinationCenter to map with connections of Hub
     *
     * @param name Name of the VaccinationCenter
     * @param center Pointer to VaccinationCenter object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     * REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized")
     * REQUIRE(fcentra.find(name) == fcentra.end() ,"VaccinationCenter must not exist yet in map");
     *
     * @post
     * ENSURE( fcentra.find(name) != fcentra.end(),"VaccinationCenter must exist in map");
     */
    void addCenter(const std::string &name, VaccinationCenter* center);

    /**
     * \brief Update the amount of vaccins in Hub
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Hub must be properly initialized")
     */
    void updateVaccins();

    std::map<std::string, Vaccin*> getVaccinZero();

    /**
    * \brief Distribute vaccin fairly between the centers and over the days (withoud considering 2nd vaccination)
    *
    * @param vaccin: viccin that needs to be disdributed
    * @param currentDay: the current day of the simmulation
    * @param stream: stream Output stream
    *
    * @pre
    * REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    * REQUIRE(currentDay >= 0, "currentDay cannot be negative");
    * REQUIRE(this->getVaccins().find(vaccin->getType()) != this->getVaccins().end(), "Given vaccin must exist");
    *
    */
    void distributeVaccinsFair(Vaccin* vaccin, int currentDay, std::ostream &stream);

    /**
    * \brief Distribute required vaccins over an vaccinationCenter
    *
    * @param vaccinationCenter: gives this center the required vaccins
    * @param stream: stream Output stream
    *
    * @pre
    * REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    * REQUIRE(vaccinationCenter->properlyInitialized(), "VaccinationCenter must be properly initialized");
    *
    */
    void distributeRequeredVaccins(VaccinationCenter* vaccinationCenter, std::ostream &stream);

    /**
    * \brief Gives the most suitable VaccinationCenter for a specified amount of a vaccin type
    *
    * @param vaccinCount: amount of Vaccins
    * @param vaccin: vaccin type
    *
    * @pre
    * REQUIRE(properlyInitialized(), "Hub must be properly initialized");
    * REQUIRE(vaccin->properlyInitialized(), "Vaccin must be properly initialized");
    */
    VaccinationCenter* mostSuitableVaccinationCenter(int vaccinCount, Vaccin* vaccin);

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
     * @param stream Output stream
     * @param counterFigures Current number of figures already in the .ini file
     * @param counterHub Current number of Hubs already in the .ini file
     *
     * @return Cartesian coordinates of Hub as string in .ini file format
     */
    std::string generateIni(std::ofstream & stream, int & counterFigures, int & counterHub) const;
};

#endif //TTT_HUB_H
