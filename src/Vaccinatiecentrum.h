//
// Created by stein on 4/03/2021.
//

#ifndef TTT_VACCINATIECENTRUM_H
#define TTT_VACCINATIECENTRUM_H
#include <vector>
#include <map>
#include <string>
#include "XMLReader.h"

using namespace std;

class Vaccinatiecentrum {
    string naam;
    string adres;
    int inwoners;
    int capacitiet;
public:
    Vaccinatiecentrum(string& naam, string& adres, int inwoners, int capacitiet);
    string getNaam();
    string getAdres();
    int getInwoners();
    int getcapacitiet();
};


#endif //TTT_VACCINATIECENTRUM_H
