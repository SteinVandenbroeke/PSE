/**
 * @file Vaccin.cpp
 * @brief This file contains the definitions of the members of the Vaccin class
 * @author Stein Vandenbroeke
 * @date 09/04/2021
 */

#include "Vaccin.h"

bool VaccinInHub::properlyInitialized() const {

    return VaccinInHub::_initCheck == this;
}

VaccinInHub::VaccinInHub(std::string type, const int delivery, const int interval, const int transport, const int renewal,
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
    this->fvaccinRenewal = renewal;
    this->fvaccinTemperature = temp;
    this->fdelivered = 0;
    this->fvaccinAmount = this->fdelivery; //amount of vaccins = delivery on day "zero" of simulation
    _initCheck = this;

    ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
}

void VaccinInHub::copyVaccin(const VaccinInHub *v) {

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
    this->fvaccinRenewal = v->getRenewal();
    this->fvaccinTemperature = v->getTemperature();
    this->fvaccinAmount = v->getVaccin();
    this->fdelivered = v->getDelivered();

    this->_initCheck = this;
    ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
}

int VaccinInHub::getDelivery() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fdelivery;
}

void VaccinInHub::setDelivery(const int &newDelivery) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(newDelivery >= 0, "Delivery must not be negative");
    this->fdelivery = newDelivery;
}

int VaccinInHub::getInterval() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return finterval;
}

void VaccinInHub::setInterval(const int &newInterval) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(newInterval >= 0, "Interval must not be negative");
    this->finterval = newInterval;
}

int VaccinInHub::getTransport() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return ftransport;
}

void VaccinInHub::setTransport(const int &newTransport) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(newTransport >= 0, "Transport must not be negative");
    this->ftransport = newTransport;
}

int VaccinInHub::getVaccin() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccinAmount;
}

int VaccinInHub::getDelivered() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fdelivered;
}

void VaccinInHub::updateVaccins() {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    fvaccinAmount += fdelivery;
}

void VaccinInHub::updateVaccinsTransport(int transportAmount) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(transportAmount % this->getTransport() == 0, "Wrong transport amount, Cargo amount must be a int");

    // Update amount of vaccins in Hub and Center
    fvaccinAmount -= transportAmount;
    fdelivered += transportAmount;
}

const std::string &VaccinInHub::getType() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return ftype;
}

int VaccinInHub::getRenewal() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccinRenewal;
}

void VaccinInHub::setRenewal(const int &newRenewal) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(newRenewal >= 0, "Interval must not be negative");
    this->fvaccinRenewal = newRenewal;
}

int VaccinInHub::getTemperature() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccinTemperature;
}

bool VaccinInHub::checkUnderZero() {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return getTemperature() < 0;
}
