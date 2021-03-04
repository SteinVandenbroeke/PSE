//
// Created by stein on 4/03/2021.
//

#include "Hub.h"

Hub::Hub(int levering, int interval, int transport) {
    this->vaccins = levering;
    this->levering = levering;
    this->interval = interval;
    this->transport = transport;
}

void Hub::addVaccinatiecentrum(string naam, Vaccinatiecentrum *centrum) {
    this->centra[naam] = centrum;
}

void Hub::updateVaccinatiecentrum(string naam, Vaccinatiecentrum *centrum) {
    if(this->centra[naam]){
        delete this->centra[naam];
        this->centra[naam] = centrum;
    }
}

void Hub::print(ofstream &stream) {
    stream << "Hub (" << vaccins << " vaccins)\n";
    for(map<string,Vaccinatiecentrum*>::iterator it = this->centra.begin(); it != this->centra.end(); it++){
        stream << " -> " << it->first << " (" << (it->second)->getVaccins() << " vaccins)\n";
    }
    stream << "\n";
}
