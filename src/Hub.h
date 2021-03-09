/**
 * @file Hub.h
 * @brief This header file contains the declarations and the members of the Hub class
 * @author Stein Vandenbroeke
 * @date 04/03/2012
 */

#ifndef TTT_HUB_H
#define TTT_HUB_H

#include <map>
#include "DesignByContract.h"
#include "VaccinationCenter.h"

class Hub {

private:
    int fdelivery;
    int finterval;
    int ftransport;
    int fvaccin;
    std::map<std::string, VaccinationCenter*> fcentra;
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
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    Hub(int fdelivery, int finterval, int ftransport);

    /**
     * \brief Check whether the Hub object is properly initialised
     *
     * @return true when object is properly initialised, false when not
     */
    bool properlyInitialized() const;

    void addCenter(const std::string &name, VaccinationCenter* center);

    void updateCenter(const std::string &name, VaccinationCenter* center);

    void print(std::ofstream& stream);
};

#endif //TTT_HUB_H
