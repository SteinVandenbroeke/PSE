#include <iostream>
#include <unistd.h>
#include "Simulation.h"

int main() {
    Simulation s;
    try{
        s.importXmlFile("simulatiecentra.xml");

        s.automaticSimulation(1, std::cout);
        s.exportFile("test.txt");

    }
    catch (Exception ex) {
        cerr << ex.value() << endl;
    }
    return 0;
}