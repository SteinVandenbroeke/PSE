#include <iostream>
#include <unistd.h>
#include "Simulation.h"

int main() {
    Simulation s;
    try{
        s.importXmlFile("simulatiecentra.xml");

        ofstream exportFile;
        exportFile.open("test1");

        s.automaticSimulation(15,03,2021, exportFile);

        while (true) {
            s.simulateTransport(std::cout);
            s.simulateVaccination(std::cout);

            s.exportFile("test.txt");
            s.increaseIterator();
            sleep(4);
        }
    }
    catch (Exception ex) {
        cerr << ex.value() << endl;
    }
    return 0;
}