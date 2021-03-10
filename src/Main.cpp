#include <iostream>
#include <unistd.h>
#include "Simulation.h"

int main() {
    Simulation s;
    try{
        s.importXmlFile("simulatiecentra.xml");


        while (true) {

            s.simulateTransport();

            sleep(1);
        }

    }
    catch (Exception ex) {
        cerr << ex.value() << endl;
    }
    return 0;
}