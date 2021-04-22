/**
 * @file Main.cpp
 * @brief This file is used to run the system
 * @author Pablo Deputter
 * @date 10/03/2021
 */

#include <iostream>
#include <unistd.h>
#include "Simulation.h"
#include <sstream>

int main() {
    Simulation s;
    try{
        s.importXmlFile("simulate_spec_renewal.xml", "knownTags_spec_10.xml" ,std::cerr);
        std::cout << "Done importing" << std::endl;
        s.automaticSimulation(20, std::cout);

//        s.generateIni("export.ini");



/*
        s.simulateTransport(0, std::cout);
        s.getHub()[0]->printGraphical(std::cout);
        s.simulateVaccination(std::cout);

        for (std::map<std::string, VaccinationCenter*>::iterator it = s.getHub()[0]->getCentra().begin(); it != s.getHub()[0]->getCentra().end(); it++) {
            it->second->updateRenewal();
        }

        for (std::map<std::string, Vaccin*>::iterator it = s.getHub()[0]->getVaccins().begin(); it != s.getHub()[0]->getVaccins().end(); it++) {
            it->second->updateVaccins();
        }
        s.getHub()[0]->printGraphical(std::cout);

        s.simulateTransport(1, std::cout);
        s.simulateVaccination(std::cout);
        std::cout << "############# VACCINATION REPORT #############" << std::endl;
       s.getHub()[0]->printGraphical(std::cout);
        s.simulateTransport(2, std::cout);
        s.getHub()[0]->printGraphical(std::cout);
        s.simulateVaccination(std::cout);
        s.getHub()[0]->printGraphical(std::cout);
        for (std::map<std::string, VaccinationCenter*>::iterator it = s.getHub()[0]->getCentra().begin(); it != s.getHub()[0]->getCentra().end(); it++) {
            it->second->updateRenewal();
        }
        s.simulateVaccination(std::cout);

*/
    }
    catch (Exception ex) {
        std::cerr << ex.value() << std::endl;
    }
    return 0;
}