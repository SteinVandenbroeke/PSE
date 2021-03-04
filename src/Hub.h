//
// Created by stein on 4/03/2021.
//

#ifndef TTT_HUB_H
#define TTT_HUB_H
#include <map>
#include <string>
#include "Vaccinatiecentrum.h"
#include <iostream>
#include <fstream>

class Hub {
int levering;
int interval;
int transport;
int vaccins;
map<string, Vaccinatiecentrum*> centra;
public:
    Hub(int levering, int interval, int transport);
    void addVaccinatiecentrum(string naam, Vaccinatiecentrum* centrum);
    void updateVaccinatiecentrum(string naam, Vaccinatiecentrum* centrum);
    void print(ofstream& stream);
};


#endif //TTT_HUB_H
