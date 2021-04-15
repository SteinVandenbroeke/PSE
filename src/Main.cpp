/**
 * @file Main.cpp
 * @brief This file is used to run the system
 * @author Pablo Deputter
 * @date 10/03/2021
 */

#include <iostream>
#include <unistd.h>
#include "Simulation.h"

int main() {
    Simulation s;
    try{
        s.importXmlFile("simulate_spec_renewal.xml", "knownTags_spec_10.xml" ,std::cerr);
        std::cout << "Done importing" << std::endl;

        s.simulateTransport(std::cout);
        s.getHub()[0]->printGraphical(std::cout);
        s.simulateVaccination(std::cout);
        for (std::map<std::string, VaccinationCenter*>::iterator it = s.getHub()[0]->getCentra().begin(); it != s.getHub()[0]->getCentra().end(); it++) {
            it->second->updateRenewal();
        }
        std::cout << "############# VACCINATION REPORT #############" << std::endl;
        s.getHub()[0]->printGraphical(std::cout);

        s.simulateVaccination(std::cout);
        s.getHub()[0]->printGraphical(std::cout);
        s.simulateTransport(std::cout);
        s.getHub()[0]->printGraphical(std::cout);
        s.simulateVaccination(std::cout);
        s.getHub()[0]->printGraphical(std::cout);

    }
    catch (Exception ex) {
        std::cerr << ex.value() << std::endl;
    }
    return 0;
}