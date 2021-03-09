#include <iostream>
#include "Simulation.h"

int main() {
    Simulation s;
    try{
        s.readXmlFile("simulatiecentra.xml");
        s.exportFile("test");
    }
    catch (Exception ex) {
        cerr << ex.value() << endl;
    }
    return 0;
}