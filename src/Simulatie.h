//
// Created by stein on 4/03/2021.
//

#ifndef TTT_SIMULATIE_H
#define TTT_SIMULATIE_H
#include <vector>
#include "XMLReader.h"
#include "Vaccinatiecentrum.h"
#include "Hub.h"
#include <iostream>
#include <fstream>

class Simulatie {
    vector<Vaccinatiecentrum*> centra;
    Hub* hub;
public:
    void readXmlFile(const char* pad);
    void exportFile(const char* pad);
};


#endif //TTT_SIMULATIE_H
