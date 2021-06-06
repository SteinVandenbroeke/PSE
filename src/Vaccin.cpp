/**
 * @file Vaccin.cpp
 * @brief This file contains the definitions of the members of the Vaccin class
 * @author Stein Vandenbroeke
 * @date 09/04/2021
 */

#include "Vaccin.h"

bool Vaccin::properlyInitialized() const {
    return _initCheck == this;
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
    ENSURE(this->ftype == type, "type is not set to value");
    ENSURE(this->fdelivery == delivery, "delivery is not set to value");
    ENSURE(this->finterval == interval, "interval is not set to value");
    ENSURE(this->ftransport == transport, "transport is not set to value");
    ENSURE(this->fvaccinRenewal == renewal, "vaccinRenewal is not set to value");
    ENSURE(this->fvaccinTemperature == temp, "vaccinTemperature is not set to value");
    ENSURE(this->fvaccinAmount == delivery, "vaccinAmount is the same value as delivery");
    ENSURE(this->fdelivered == 0, "delivered must be 0");
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
    ENSURE(this->ftype == v->getType(), "type must be the same!");
    ENSURE(this->fdelivery == v->getDelivery(), "delivery must be the same");
    ENSURE(this->finterval == v->getInterval(), "interval must be the same");
    ENSURE(this->ftransport == v->getTransport(), "transport must be the same");
    ENSURE(this->fvaccinRenewal == v->getRenewal(), "vaccinRenewal must be the same");
    ENSURE(this->fvaccinTemperature == v->getTemperature(), "vaccinTemperature must be the same");
    ENSURE(this->fvaccinAmount == v->getVaccin(), "vaccinAmount must be the same");
    ENSURE(this->fdelivered == v->getDelivered(), "delivered must be the same");
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

int Vaccin::getVaccin() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccinAmount;
}

const std::string &Vaccin::getType() const {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return ftype;
}

int Vaccin::getTemperature() const {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccinTemperature;
}

int Vaccin::getRenewal() const {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccinRenewal;
}

int VaccinInHub::getDelivered() const {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fdelivered;
}

void VaccinInHub::updateVaccins() {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    fvaccinAmount += fdelivery;
    std::cout << fvaccinAmount <<std::endl;
    ENSURE(this->getVaccin() >= this->getDelivery(), "The amount of vaccins must be bigger delivery amount (fvaccin += fdelivery)");
}

void VaccinInHub::updateVaccinsTransport(int transportAmount) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(transportAmount % this->getTransport() == 0, "Wrong transport amount, Cargo amount must be a int");

    // Update amount of vaccins in Hub and Center
    fvaccinAmount -= transportAmount;
    fdelivered += transportAmount;
    ENSURE(this->getVaccin() >= 0, "fvaccinAmount can not be negative (fvaccinAmount -= transportAmount)");
    ENSURE(this->getDelivered() >= transportAmount, "fdelivered must be lager then transportAmount (this->getDelivered() >= transportAmount)");
}

void VaccinInHub::setRenewal(const int &newRenewal) {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(newRenewal >= 0, "Interval must not be negative");
    this->fvaccinRenewal = newRenewal;
}

bool VaccinInHub::checkUnderZero() {

    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return getTemperature() < 0;
}

VaccinInHub::VaccinInHub() {
    this->_initCheck = this;
    ENSURE(properlyInitialized(), "Vaccin must end in properlyInitialized state");
}

void VaccinInCenter::copyVaccin(const VaccinInCenter *v) {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(v->ftype != "", "Vaccin type must not be empty");
    REQUIRE(v->fvaccinAmount >= 0, "Vaccin must not be negative");
    REQUIRE(v->fvaccinRenewal >= 0, "Vaccin renewal must not be negative");

    this->ftype = v->getType();
    this->fvaccinTemperature = v->getTemperature();
    this->fvaccinAmount = v->getVaccin();
    this->fvaccinRenewal = v->getRenewal();
    this->ftracker = v->getTracker();

    this->_initCheck = this;
    ENSURE(properlyInitialized(), "Copy constructor must end in properlyInitialized state");
    ENSURE(this->ftype == v->getType(), "type must be the same!");
    ENSURE(this->fvaccinRenewal == v->getRenewal(), "vaccinRenewal must be the same");
    ENSURE(this->fvaccinTemperature == v->getTemperature(), "vaccinTemperature must be the same");
    ENSURE(this->fvaccinAmount == v->getVaccin(), "vaccinAmount must be the same");
}

int &VaccinInCenter::getVaccinAmount() {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return fvaccinAmount;
}

const std::map<int, int> &VaccinInCenter::getTracker() const {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return ftracker;
}

std::map<int, int> &VaccinInCenter::getTracker() {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return ftracker;
}

void VaccinInCenter::addDay(){
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    std::map<int, int> newTracker;
    newTracker[0] = 0;
    for (std::map<int, int>::iterator it = ftracker.begin(); it != ftracker.end(); it++){
        if(it->first + 1 <= 0) {
            newTracker[it->first + 1] = it->second;
        }
        else{
            newTracker[0] += it->second;
        }
    }
    ftracker = newTracker;
}

void VaccinInCenter::insertRequiredDay(int day, int requiredPeople){
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    REQUIRE(requiredPeople >= 0, "Amount of peaple can not be negative");
    if(requiredPeople > 0){
        ftracker[day] += requiredPeople;
    }
    ENSURE(requiredPeople == 0 || ftracker.find(day) != ftracker.end(),"Day not added");
}

bool VaccinInCenter::isRenewal() const {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    return this->fvaccinRenewal != 0;
}

int VaccinInCenter::totalFirstVaccination() const  {
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    int total = 0;
    for (std::map<int, int>::const_iterator it = ftracker.begin(); it != ftracker.end(); it++){
        total += it->second;
    }
    return total;
}

void VaccinInCenter::removeVaccin(){
    REQUIRE(properlyInitialized(), "Vaccin must be properly initialized");
    this->fvaccinAmount = 0;
    ENSURE(this->getVaccinAmount() == 0, "fvaccinAmount is not set to 0");
}

VaccinInCenter::VaccinInCenter(const std::string &vaccinType, int vaccinTemperature, int vaccinRenewal,
                               int vaccinAmount)  {
    this->ftype = vaccinType;
    this->fvaccinTemperature = vaccinTemperature;
    this->fvaccinRenewal = vaccinRenewal;
    this->fvaccinAmount = vaccinAmount;
    this->_initCheck = this;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
    ENSURE(this->ftype == vaccinType, "type is not set to given value");
    ENSURE(this->fvaccinRenewal == vaccinRenewal, "vaccinRenewal is not set to given value");
    ENSURE(this->fvaccinTemperature == vaccinTemperature, "vaccinTemperature is not set to given value");
    ENSURE(this->fvaccinAmount == vaccinAmount, "vaccinAmount is not set to given value");
}

VaccinInCenter::VaccinInCenter() {
    this->_initCheck = this;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
