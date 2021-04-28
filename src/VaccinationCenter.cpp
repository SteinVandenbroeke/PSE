/**
 * @file VaccinationCenter.cpp
 * @brief This file contains the definitions of the members of the VaccinationCenter class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#include "VaccinationCenter.h"
#include "Vaccin.h"

VaccinationCenter::VaccinationCenter(const std::string &fname, const std::string &faddress, int fpopulation
                                     ,int fcapacity) :
    fname(fname), faddress(faddress), fpopulation(fpopulation),fcapacity(fcapacity){

    REQUIRE(fname.length() > 0, "Name can't be empty");
    REQUIRE(faddress.length() > 0, "Adres can't be empty");
    REQUIRE(fpopulation >= 0, "Negative population'");
    REQUIRE(fcapacity >= 0, "Negative capacity");

    _initCheck = this;
    fvaccinated = 0;
    fvaccinsType.clear();

    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool VaccinationCenter::properlyInitialized() const {

    return VaccinationCenter::_initCheck == this;
}

const std::string &VaccinationCenter::getName() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fname;
}

const std::string &VaccinationCenter::getAddress() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->faddress;
}

int VaccinationCenter::getPopulation() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fpopulation;
}

int VaccinationCenter::getCapacity() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fcapacity;
}

int VaccinationCenter::getVaccins() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    int totalVaccins = 0;

    for (std::map<const std::string, vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {
        totalVaccins += it->second->getVaccinAmount();
    }
    ENSURE(checkAmountVaccins(), "Wrong amount of vaccins");
    return totalVaccins;
}

bool VaccinationCenter::checkAmountVaccins() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    int totalVaccins = 0;
    for (std::map<const std::string, vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {
        totalVaccins += it->second->getVaccinAmount();
    }
    return totalVaccins <= getCapacity() * 2;
}

int VaccinationCenter::getVaccinated() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    return this->fvaccinated;
}

void VaccinationCenter::addVaccins(const int amount, const Vaccin* vaccin) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(amount + this->getVaccins() <= (this->getCapacity() * 2),
                "Amount of vaccins must not exceed capacity of Center");

    if (this->fvaccinsType.find(vaccin->getType()) == this->fvaccinsType.end()) {

        VaccinationCenter::vaccinType* vaccinStruct = new VaccinationCenter::vaccinType(vaccin->getType(),
                                                                                       vaccin->getTemperature(),
                                                                                       vaccin->getRenewal(), 0);

        this->fvaccinsType.insert(std::make_pair(vaccinStruct->getVaccinType(), vaccinStruct));
    }
    this->fvaccinsType.find(vaccin->getType())->second->getVaccinAmount() += amount;

    ENSURE(checkAmountVaccins(), "Amount of vaccins must not exceed capacity of Center");
}

int VaccinationCenter::calculateVaccinationAmount() const {

    int notVaccinated = this->fpopulation - (this->fvaccinated + totalWaitingForSeccondPrik());
    int smallest = std::min(this->getVaccins(), this->getCapacity());
    return std::min(smallest, notVaccinated);
}

int VaccinationCenter::calculateVaccinationAmount(const VaccinationCenter::vaccinType* vaccin, int vaccinsUsed) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int notVaccinated = this->fpopulation - (this->fvaccinated + totalWaitingForSeccondPrik());
    int smallest = std::min(vaccin->getVaccinAmount(), this->getCapacity() - vaccinsUsed);
    return std::min(smallest, notVaccinated);
}

int VaccinationCenter::calculateVaccinationAmountSecondShot(VaccinationCenter::vaccinType* vaccin, const int firstShot,
                                                            int & vaccinated, int & vaccinsUsed) {

    REQUIRE(firstShot >= 0, "Amount of people recieved first shot must not be negative");
    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int smallest = std::min(vaccin->getVaccinAmount(), this->fcapacity);
    smallest = std::min(smallest, firstShot);
    int secondShot = std::min(smallest, this->fcapacity - vaccinsUsed);
    secondShot = std::min(secondShot, this->fpopulation - this->fvaccinated);

    vaccinated += secondShot;
    vaccinsUsed += secondShot;
    vaccin->getVaccinAmount() -= secondShot;

    // No vaccins left over after serving the second shot
    if (secondShot >= vaccin->getVaccinAmount()) {
        return secondShot;
    }
    else {
        int notVaccinated = this->fpopulation - (this->fvaccinated + vaccinated);

        if (notVaccinated - totalWaitingForSeccondPrik() - vaccinsUsed >= 0) {

            int smallest_ = std::min(vaccin->getVaccinAmount(), this->fcapacity - secondShot);
            int newFirstShot = std::min(smallest_, notVaccinated - totalWaitingForSeccondPrik() - vaccinated);
            newFirstShot = std::min(newFirstShot, this->fcapacity - vaccinsUsed);

            if (newFirstShot != 0) {
                vaccin->insertRequiredDay(vaccin->getVaccinRenewal() * (-1), newFirstShot);
            }
            vaccin->getVaccinAmount();
            vaccinsUsed += newFirstShot;
        }
        return secondShot;
    }
}

void VaccinationCenter::updateRenewal() {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    for (std::map<const std::string, VaccinationCenter::vaccinType*>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
            (it)->second->addDay();
    }
}

std::map<const std::string, VaccinationCenter::vaccinType*> VaccinationCenter::getVaccin(bool zeroVaccin) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    std::map<const std::string, vaccinType*> zeroVaccins;

    for (std::map<const std::string, VaccinationCenter::vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {
        if((it->second->getVaccinTemperature() < 0 && zeroVaccin) || (it->second->getVaccinTemperature() > 0 && !zeroVaccin)) {
            zeroVaccins.insert(std::make_pair(it->first, it->second));
        }
    }
    return zeroVaccins;
}

void VaccinationCenter::vaccinateCenter(std::map<const std::string, vaccinType*> vaccinsType, int & vaccinated, int & vaccinsUsed) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(checkAmountVaccins(), "Amount of vaccins must not exceed capacity");

    for (std::map<const std::string, VaccinationCenter::vaccinType*>::iterator it = vaccinsType.begin(); it != vaccinsType.end(); it++) {

        // Vaccin with renewal
        if (it->second->isRenewal()) {

            // Population did not yet get a first Vaccin shot
            if (it->second->getTracker().empty()) {

                int vaccinAmount = calculateVaccinationAmount(it->second, vaccinsUsed);
                vaccinsUsed += vaccinAmount;
                it->second->getVaccinAmount() -= vaccinAmount;
                // Insert in tracker-map
                it->second->insertRequiredDay(it->second->getVaccinRenewal() * (-1), vaccinAmount);
            }

            // Population has already recieved a first Vaccins shot
            else {
                std::map<int, int> & firstShot = it->second->getTracker();

                for (std::map<int, int>::iterator ite = firstShot.begin(); ite != firstShot.end(); ite++) {

                    // Renewal interval is over
                    if (ite->first == 0) {

                        // Return secondShot amount
                        int vaccinAmount = calculateVaccinationAmountSecondShot(it->second, ite->second,
                                                                                vaccinated, vaccinsUsed);
                        // Remove from map
                        ite->second -= vaccinAmount;
                    }
                }
            }
        }

        // Vaccin without renewal
        else {
            int vaccinAmount = calculateVaccinationAmount(it->second, vaccinsUsed);
            vaccinated += vaccinAmount;
            vaccinsUsed += vaccinAmount;
            it->second->getVaccinAmount() -= vaccinAmount;
        }
    }

    // Vaccinated will be dealt with on caller side
}

void VaccinationCenter::print(std::ostream &stream) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    stream << this->fname << ": " << this->fvaccinated << " inwoners gevaccineerd, nog ";
    stream << (this->fpopulation - this->fvaccinated) << " inwoners niet gevaccineerd\n";
}

void VaccinationCenter::printGraphical(std::ostream &stream) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int perVaccin = ToPercent(this->getVaccins(), fcapacity);
    int perVaccinated = ToPercent(fvaccinated, fpopulation);

    if(perVaccin > 100) {
        perVaccin = 100;
    }

    stream << this->fname << ":" << "\n";
    stream << "\t" << "- geavaccineerd " << ProgressBar(perVaccinated, 20) << " " <<  perVaccinated << "%" << ": " << fvaccinated << "/" << fpopulation << "\n";
//    stream << "\t \t- " << "Totaal volledig: " << ": " << fvaccinated << "/" << fpopulation << "\n";
    stream << "\t" << "- " << "vaccins       " << ProgressBar(perVaccin, 20)   << " "<< perVaccin << "%" <<"\n";
    for (std::map<const std::string, VaccinationCenter::vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {
        stream << "\t \t- " << it->first << ": " << it->second->getVaccinAmount() << "\n";
        if(it->second->totalFirstVaccination() != 0)
            stream << "\t \t \t- " << "Eerste prik: " << ": " << it->second->totalFirstVaccination() << "\n";
    }
}

std::string VaccinationCenter::stockToSize() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    // Min size
    if (this->getVaccins() == 0) {

        return "scale = 0.05\n";
    }
    // Max size
    else if (this->getVaccins() == this->fcapacity) {

        return "scale = 0.12\n";
    }
    else {
        double scale = 0.05;
        scale += 0.07 * ToPercent(this->getVaccins(), fcapacity) / 100;
        return "scale = " + ToString(scale) + "\n";
    }
}

std::string VaccinationCenter::vaccinatedToColor() const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    // Red
    if (this->fvaccinated == 0) {

        return "color = (1, 0, 0)\n";
    }
    // Green
    else if (this->fvaccinated == this->fpopulation) {

        return "color = (0, 1, 0)\n";
    }
    // Mix between Red and Green
    else {

        double a = static_cast<double>(fvaccinated) / fpopulation;
        a = static_cast<double>(a * 100);

        double red = 1.0 - a / 100;
        double green = 0.0 + a / 100;
        return "color = (" + ToString(red) + ", " + ToString(green) + ", 0)\n";
    }
}

std::pair<double, double> VaccinationCenter::generateIni(std::ofstream & stream, int& counterFigures,
                                                         int & counterCenter, const double & maxHubX) const {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    std::string x;

    x.append("[Figure" + ToString(counterFigures) + "]" + "\n");
    counterFigures++;
    counterCenter++;
    x.append("type = \"Cube\"\n");
    x.append(this->stockToSize());
    x.append("rotateX = 0\n");
    x.append("rotateY = 0\n");
    x.append("rotateZ = 0\n");
    double positionX = static_cast<double>(counterCenter) / 1.5 + (0 * 2);
    x.append("center = (" + ToString(positionX) + ", " + "1.2" + ", " + "0" + ")\n");
    x.append(this->vaccinatedToColor());

    x.append("\n");

    stream << x;
    return std::make_pair(positionX, 1.2);
}

std::map<std::string, int> VaccinationCenter::requiredAmountVaccinType() {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    std::map<std::string, int> requiredVaccin;
    for(std::map<const std::string, VaccinationCenter::vaccinType*>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
        if(requiredAmountVaccin(it->second) != 0) {
            requiredVaccin[it->first] = requiredAmountVaccin(it->second);
        }
    }
    return requiredVaccin;
}

void VaccinationCenter::vaccinateCenter(std::ostream &stream) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(checkAmountVaccins(), "Amount of vaccins must not exceed capacity");

    int vaccinated = 0;
    int vaccinsUsed = 0;

    vaccinateCenter(getVaccin(true), vaccinated, vaccinsUsed);

    vaccinateCenter(getVaccin(false), vaccinated, vaccinsUsed);

    this->fvaccinated += vaccinated;

    if(vaccinsUsed == 0 && this->fvaccinated + this->totalWaitingForSeccondPrik() == this->getPopulation()){
        for(std::map<const std::string, vaccinType*>::const_iterator it = fvaccinsType.begin();
        it != fvaccinsType.end(); it++) {
            if (it->second->totalFirstVaccination() <= 0 && it->second->getVaccinAmount() > 0) {

                stream << "Er werden " << it->second->getVaccinAmount() << " onodige vaccins van " << it->second->getVaccinType();
                stream << " verwijderd." << std::endl;
                it->second->removeVaccin();
            }
        }
    }

    ENSURE(this->getVaccinated() <= this->getPopulation(), "Peaple that are vaccinated can not be more than the population");

    if (vaccinated == 0) {
        return;
    }
    stream << "Er werden " << vaccinated << " inwoners gevaccineerd in " << this->fname << ".\n";
}

int VaccinationCenter::getOpenVaccinStorage(Vaccin* vaccin) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");
    REQUIRE(vaccin->properlyInitialized(), "VaccinationCenter must be properly initialized");


    std::map<std::string, int> requiredVaccins = this->requiredAmountVaccinType();
    if((this->getPopulation() - this->getVaccinated() - this->totalWaitingForSeccondPrik()) <= this->getVaccins()
    && (requiredVaccins.find(vaccin->getType()) == requiredVaccins.end() || requiredVaccins.find(vaccin->getType())->second == 0)){
        //indien er voldoende vaccins op vooraad zijn om iedereen te vaccineren
        return 0;
    }

    std::map<const std::string, vaccinType*> zeroVaccins;
    int openVaccinStorageTotal = this->getCapacity() * 2;
    int openVaccinStorage = openVaccinStorageTotal;
    if(vaccin->checkUnderZero()){
        openVaccinStorage = this->getCapacity();
    }

    for (std::map<const std::string, VaccinationCenter::vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
        if((it->second->getVaccinTemperature() < 0 && vaccin->checkUnderZero()) || (it->second->getVaccinTemperature() > 0 && !vaccin->checkUnderZero())) {
            openVaccinStorage -= it->second->getVaccinAmount();
        }
        openVaccinStorageTotal -= it->second->getVaccinAmount();
    }

    int minimum = std::min(openVaccinStorageTotal, openVaccinStorage);
    return std::min(minimum, std::max(this->getPopulation() - this->getVaccinated() - this->getVaccins(), vaccin->getTransport()));
}

int VaccinationCenter::requiredAmountVaccin(VaccinationCenter::vaccinType *vaccin) {

    REQUIRE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int needed = vaccin->getTracker().find(0)->second - vaccin->getVaccinAmount();
    if(needed < 0) {
        needed = 0;
    }

    return needed;
}

VaccinationCenter::~VaccinationCenter() {

    ENSURE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    for (std::map<const std::string, VaccinationCenter::vaccinType*>::iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++) {
        delete it->second;
    }
}

int VaccinationCenter::totalWaitingForSeccondPrik() const {

    ENSURE(properlyInitialized(), "VaccinationCenter must be properly initialized");

    int total = 0;
    for(std::map<const std::string, vaccinType*>::const_iterator it = fvaccinsType.begin(); it != fvaccinsType.end(); it++){
        total += it->second->totalFirstVaccination();
    }
    return total;
}
