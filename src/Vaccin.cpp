/**
 * @file Vaccin.cpp
 * @brief This file contains the definitions of the members of the Vaccin class
 * @author Stein Vandenbroeke
 * @date 09/04/2021
 */

#include "Vaccin.h"

bool Vaccin::properlyInitialized() const {

    return Vaccin::_initCheck == this;
}

Vaccin::Vaccin(std::string type, const int delivery, const int interval, const int transport, const int renewal,
               const int temp) {

    REQUIRE(type != "", "type can't be empty");
    REQUIRE(delivery >= 0, "Delivery can't be negative");
    REQUIRE(interval >= 0, "Interval can't be negative");
    REQUIRE(transport >= 0, "Transport can't be negative");
    REQUIRE(renewal >= 0, "Renewal can't be negative");

    this->ftype = type;
    this->fdelivery = delivery;
    this->finterval = interval;
    this->ftransport = transport;
    this->frenewal = renewal;
    this->ftemperature = temp;
    this->fdelivered = 0;
    this->fvaccin = this->fdelivery; //amount of vaccins = delivery on day "zero" of simulation
    _initCheck = this;

    ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
}

void Vaccin::copyVaccin(const Vaccin *v) {

    REQUIRE(v->properlyInitialized(), "Vaccin must be properly initialized");

    REQUIRE(v->getType() != "", "Type must not be negative");
    REQUIRE(v->getDelivery() >= 0, "Delivery must not be negative");
    REQUIRE(v->getInterval() >= 0, "Interval must not be negative");
    REQUIRE(v->getTransport() >= 0, "Transport must not be negative");
    REQUIRE(v->getRenewal() >= 0, "Renewal must not be negative");
//    REQUIRE(v->getVaccin() >= 0, "Amount of vaccins must not be negative");

    this->ftype = v->getType();
    this->fdelivery = v->getDelivery();
    this->finterval = v->getInterval();
    this->ftransport = v->getTransport();
    this->frenewal = v->getRenewal();
    this->ftemperature = v->getTemperature();
    this->fvaccin = v->getVaccin();
    this->fdelivered = v->getDelivered();

    this->_initCheck = this;
    ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
}

int Vaccin::getDelivery() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fdelivery;
}

int Vaccin::getInterval() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return finterval;
}

int Vaccin::getTransport() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return ftransport;
}

int Vaccin::getVaccin() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccin;
}

int Vaccin::getDelivered() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fdelivered;
}

void Vaccin::updateVaccins() {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    fvaccin += fdelivery;
}

void Vaccin::updateVaccinsTransport(int transportAmount) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(transportAmount % this->getTransport() == 0, "Wrong transport amount, Cargo amount must be a int");

    // Update amount of vaccins in Hub and Center
    fvaccin -= transportAmount;
    fdelivered += transportAmount;
}

const std::string &Vaccin::getType() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return ftype;
}

int Vaccin::getRenewal() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return frenewal;
}

int Vaccin::getTemperature() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return ftemperature;
}

bool Vaccin::checkUnderZero() {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return getTemperature() < 0;
}
