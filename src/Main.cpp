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
        s.importXmlFile("simulatiecentra.xml", "knownTags.xml");

        s.automaticSimulation(4, std::cout);
        s.exportFile("test.txt");

    }
    catch (Exception ex) {
        std::cerr << ex.value() << std::endl;
    }
    return 0;
}