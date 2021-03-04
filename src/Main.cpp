#include <iostream>
#include "Simulatie.h"

using namespace std;

int main() {
    Simulatie s;
    try{
        s.readXmlFile("simulatiecentra.xml");
        s.exportFile("test");
    }
    catch (Exception ex) {
        cerr << ex.value() << endl;
    }
    return 0;
}