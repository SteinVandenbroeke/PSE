#include <iostream>
#include "Vaccinatiecentra.h"

using namespace std;

int main() {
    Vaccinatiecentra* vaccinatiecentra = new Vaccinatiecentra();
    vaccinatiecentra->loadXml("simulatiecentra.xml");
    return 0;
}