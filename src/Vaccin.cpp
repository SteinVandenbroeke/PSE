//
// Created by stein on 7/04/2021.
//

#include "Vaccin.h"

bool Vaccin::properlyInitialized() const {
    return Vaccin::_initCheck == this;
}

Vaccin::Vaccin(std::string& type, int delivery, int interval, int transport) {
    REQUIRE(type != "", "type can't be empty");
    REQUIRE(delivery >= 0, "Delivery can't be negative");
    REQUIRE(interval >= 0, "Interval can't be negative");
    REQUIRE(transport >= 0, "Transport can't be negative");

    this->ftype = type;
    this->fdelivery = delivery;
    this->finterval = interval;
    this->ftransport = transport;
    this->fvaccin = this->fdelivery;//amount of vaccins = delivery
    _initCheck = this;
    ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
}

int Vaccin::getDelivery() const {
    REQUIRE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
    return fdelivery;
}

int Vaccin::getInterval() const {
    REQUIRE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
    return finterval;
}

int Vaccin::getTransport() const {
    REQUIRE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
    return ftransport;
}

int Vaccin::getVaccin() const {
    REQUIRE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
    return fvaccin;
}

void Vaccin::updateVaccins() {
    REQUIRE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
    fvaccin += fdelivery;
}

void Vaccin::updateVaccinsTransport(int transportAmount) {
    REQUIRE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
    // Update amount of vaccins in Hub and Center
    fvaccin -= transportAmount;

}

