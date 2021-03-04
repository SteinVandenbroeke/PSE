//
// Created by stein on 4/03/2021.
//

#include "Vaccinatiecentrum.h"

Vaccinatiecentrum::Vaccinatiecentrum(string& naam, string& adres, int inwoners, int capacitiet) {
    this->naam = naam;
    this->adres = adres;
    this->inwoners = inwoners;
    this->capacitiet = capacitiet;
}

string Vaccinatiecentrum::getNaam() {
    return naam;
}

string Vaccinatiecentrum::getAdres() {
    return adres;
}

int Vaccinatiecentrum::getInwoners() {
    return inwoners;
}

int Vaccinatiecentrum::getVaccins() {
    return vaccins;
}

int Vaccinatiecentrum::getCapacitiet() {
    return capacitiet;
}

int Vaccinatiecentrum::getGevaccineerd() {
    return gevaccineerd;
}

void Vaccinatiecentrum::print(ofstream &stream) {
    stream << naam << ": " << getGevaccineerd() << " gevaccineerd, nog " << (getInwoners() - getGevaccineerd()) << " inwoners niet gevaccineerd\n";
}
