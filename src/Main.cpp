#include <iostream>
#include <unistd.h>
#include "Simulation.h"

int main() {
    Simulation s;
    try{
        s.importXmlFile("simulatiecentra.xml");


        while (true) {

            s.simulateTransport();
            s.simulateVaccination();

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