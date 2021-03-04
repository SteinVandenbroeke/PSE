//
// Created by stein on 4/03/2021.
//

#ifndef TTT_VACCINATIECENTRA_H
#define TTT_VACCINATIECENTRA_H
#include <string>
#include <vector>
#include "XMLReader.h"
#include "iostream"
#include "Vaccinatiecentrum.h"

using namespace std;
class Vaccinatiecentra {
    vector<Vaccinatiecentrum*> centra;
public:
    Vaccinatiecentra();
    void loadXml(const char*);
};


#endif //TTT_VACCINATIECENTRA_H
