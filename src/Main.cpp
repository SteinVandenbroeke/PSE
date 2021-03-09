#include <iostream>
#include "Simulation.h"

int main() {
    Simulation s;
    try{
        s.importXmlFile("simulatiecentra.xml");

        s.exportFile("test.txt");
    }
    catch (Exception ex) {
        cerr << ex.value() << endl;
    }
    return 0;
}