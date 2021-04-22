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
        s.automaticSimulation(100, std::cout);

        s.generateIni("export.ini");
    }
    catch (Exception ex) {
        std::cerr << ex.value() << std::endl;
    }
    return 0;
}