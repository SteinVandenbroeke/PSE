//
// Created by stein on 4/03/2021.
//

#ifndef TTT_VACCINATIECENTRUM_H
#define TTT_VACCINATIECENTRUM_H
#include <vector>
#include <map>
#include <string>
#include "XMLReader.h"
#include <iostream>
#include <fstream>

using namespace std;

class Vaccinatiecentrum {
    string naam;
    string adres;
    int inwoners;
    int capacitiet;
    int vaccins;
    int gevaccineerd;
public:
    Vaccinatiecentrum(){};
    Vaccinatiecentrum(string& naam, string& adres, int inwoners, int capacitiet);
    string getNaam();
    string getAdres();
    int getInwoners();
    int getCapacitiet();
    int getVaccins();
    int getGevaccineerd();
    void print(ofstream &stream);
};


#endif //TTT_VACCINATIECENTRUM_H
