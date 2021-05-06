/**
 * @file VdDomainTests.cpp
 * @brief This file is used to run tests for the Hub class
 * @author Pablo Deputter
 * @date 11/03/2021
 */

#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "Simulation.h"

class VaccinDistributorDomainTests : public::testing::Test {

protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {

        ASSERT_TRUE(DirectoryExists("tests"));
        ASSERT_TRUE(DirectoryExists("tests/domainTests"));
        ASSERT_TRUE(DirectoryExists("tests/domainTests/expectedOutput"));
        ASSERT_TRUE(DirectoryExists("tests/domainTests/generatedOutput"));
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

    // Declares the variables your tests want to use.

};

// Test the default constructor of Simulation object
TEST_F(VaccinDistributorDomainTests, DefaultConstructorSimulation) {

    Simulation simulation = Simulation();

    EXPECT_TRUE(simulation.properlyInitialized());
    EXPECT_FALSE(!simulation.properlyInitialized());
    EXPECT_EQ(0, simulation.getIter());
    EXPECT_FALSE(simulation.checkSimulation());
    EXPECT_TRUE(simulation.getHub().empty());
    EXPECT_TRUE(simulation.getFcentra().empty());
}

// Test constructor of Hub object
TEST_F(VaccinDistributorDomainTests, ConstructorHub) {

    Hub hub = Hub();

    EXPECT_TRUE(hub.getVaccins().empty());
    EXPECT_TRUE(hub.getCentra().empty());
    EXPECT_TRUE(hub.properlyInitialized());
    EXPECT_FALSE(!hub.properlyInitialized());
}

// Test addCenter()
TEST_F(VaccinDistributorDomainTests, AddCenter) {

    Hub hub = Hub();

    EXPECT_TRUE(hub.getVaccins().empty());
    EXPECT_TRUE(hub.getCentra().empty());
    EXPECT_TRUE(hub.properlyInitialized());

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                  75000, 5500);
    EXPECT_TRUE(center->properlyInitialized());
    EXPECT_EQ("Flanders Expo", center->getName());
    EXPECT_EQ("Maaltekouter 1, 9051 Gent", center->getAddress());
    EXPECT_EQ(75000, center->getPopulation());
    EXPECT_EQ(5500, center->getCapacity());
    EXPECT_EQ(0, center->getVaccins());
    EXPECT_EQ(0, center->getVaccinated());

    hub.addCenter("Flanders Expo", center);
    EXPECT_FALSE(hub.getCentra().empty());
    EXPECT_TRUE(hub.getCentra().find("Flanders Expo")->first == center->getName());
    EXPECT_TRUE(hub.getCentra().find("Flanders Expo")->second == center);
}

// Test VaccinationCenter constructor with wrong values
TEST_F(VaccinDistributorDomainTests, CenterWrongValues) {

    EXPECT_DEATH(VaccinationCenter *center = new VaccinationCenter("",
                            "Maaltekouter 1, 9051 Gent",75000, 5500), "Name can't be empty");
    EXPECT_DEATH(VaccinationCenter *center = new VaccinationCenter("Flanders Expo",
                            "",75000, 5500), "Adres can't be empty");
    EXPECT_DEATH(VaccinationCenter *center = new VaccinationCenter("Flanders Expo",
                         "Maaltekouter 1, 9051 Gent",-7684, 5500), "Negative population");
    EXPECT_DEATH(VaccinationCenter *center = new VaccinationCenter("Flanders Expo",
                         "Maaltekouter 1, 9051 Gent",7684, -5500), "Negative capacity");
}

// Test center with exceptional values
TEST_F(VaccinDistributorDomainTests, ExceptionalValuesCenter) {

    EXPECT_NO_THROW(VaccinationCenter v("Park Spoor Oost", "Noordersingel 40, Antwerpen", 8000, 7500));
    EXPECT_DEATH(VaccinationCenter v("", "Noordersingel 40, Antwerpen", 8000, 7500),
                 "Name can't be empty");
    EXPECT_DEATH(VaccinationCenter v("Park Spoor Oost", "", 8000, 7500),
                 "Adres can't be empty");
    EXPECT_NO_THROW(VaccinationCenter v("Park Spoor Oost", "Noordersingel 40, Antwerpen", 0, 7500));
    EXPECT_DEATH(VaccinationCenter v("Park Spoor Oost", "Noordersingel 40, Antwerpen", -500, 7500),
                 "Negative population");
    EXPECT_NO_THROW(VaccinationCenter v("Park Spoor Oost", "Noordersingel 40, Antwerpen", 8000, 0));
    EXPECT_DEATH(VaccinationCenter v("Park Spoor Oost", "Noordersingel 40, Antwerpen", 8000, -500),
                 "Negative capacity");
}

// Double VaccinationCenter addCenter()
TEST_F(VaccinDistributorDomainTests, DoubleVaccinationCenter) {

    Hub hub = Hub();

    EXPECT_TRUE(hub.getVaccins().empty());
    EXPECT_TRUE(hub.getCentra().empty());
    EXPECT_TRUE(hub.properlyInitialized());

    VaccinationCenter *center1 = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      75000, 5500);
    EXPECT_TRUE(center1->properlyInitialized());
    hub.addCenter(center1->getName(), center1);

    VaccinationCenter *center2 = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                       10000, 1000);
    EXPECT_TRUE(center2->properlyInitialized());

    EXPECT_DEATH(hub.addCenter(center2->getName(), center2),
                 "VaccinationCenter must not exist yet in map");
}

// Test print()
TEST_F(VaccinDistributorDomainTests, PrintCenter) {

    std::string testName = "PrintCenter";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      75000, 5500);
    EXPECT_TRUE(center->properlyInitialized());
    EXPECT_EQ("Flanders Expo", center->getName());
    EXPECT_EQ("Maaltekouter 1, 9051 Gent", center->getAddress());
    EXPECT_EQ(75000, center->getPopulation());
    EXPECT_EQ(5500, center->getCapacity());
    EXPECT_EQ(0, center->getVaccins());
    EXPECT_EQ(0, center->getVaccinated());

    center->print(ostream);
    center->printGraphical(ostream);
    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Test printGraphical()
TEST_F(VaccinDistributorDomainTests, PrintGraphicalCenter) {

    std::string testName = "PrintGraphicalCenter";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      75000, 5500);
    EXPECT_TRUE(center->properlyInitialized());
    EXPECT_EQ("Flanders Expo", center->getName());
    EXPECT_EQ("Maaltekouter 1, 9051 Gent", center->getAddress());
    EXPECT_EQ(75000, center->getPopulation());
    EXPECT_EQ(5500, center->getCapacity());
    EXPECT_EQ(0, center->getVaccins());
    EXPECT_EQ(0, center->getVaccinated());

    center->printGraphical(ostream);
    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Test various functions for converting center to .ini file
TEST_F(VaccinDistributorDomainTests, CenterIni) {

    std::string testName = "CenterIni";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      20000, 5500);
    EXPECT_TRUE(center->properlyInitialized());
    EXPECT_EQ("Flanders Expo", center->getName());
    EXPECT_EQ("Maaltekouter 1, 9051 Gent", center->getAddress());
    EXPECT_EQ(20000, center->getPopulation());
    EXPECT_EQ(5500, center->getCapacity());
    EXPECT_EQ(0, center->getVaccins());
    EXPECT_EQ(0, center->getVaccinated());

    Vaccin* vaccin = new Vaccin("Pfizer", 45000, 12, 2000, 0, -15);

    EXPECT_TRUE(vaccin->properlyInitialized());
    EXPECT_EQ("Pfizer", vaccin->getType());
    EXPECT_EQ(45000, vaccin->getDelivery());
    EXPECT_EQ(12, vaccin->getInterval());
    EXPECT_EQ(2000, vaccin->getTransport());
    EXPECT_EQ(0, vaccin->getRenewal());
    EXPECT_EQ(-15, vaccin->getTemperature());
    EXPECT_TRUE(vaccin->checkUnderZero());
    EXPECT_EQ(45000, vaccin->getVaccin());

    center->addVaccins(6000, vaccin);

    EXPECT_EQ("scale = 0.1263\n", center->stockToSize());
    EXPECT_EQ("color = (1, 0, 0)\n", center->vaccinatedToColor());
    center->vaccinateCenter(ostream);
    ostream << "\n";
    EXPECT_EQ("scale = 0.0563\n", center->stockToSize());
    EXPECT_EQ("color = (0.725, 0.275, 0)\n", center->vaccinatedToColor());
    int counterFigures = 4;
    int counterCenter = 2;
    center->generateIni(ostream, counterFigures, counterCenter, 0.125);

    ostream.close();
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Test vaccin with exceptional values
TEST_F(VaccinDistributorDomainTests, VaccinExceptionalValues) {
    Vaccin v("Naam", 1, 10, 1, 2, 20);
    EXPECT_TRUE(v.properlyInitialized());

    Vaccin v1("Naam", 1, 10, 1, 2, -20);
    EXPECT_TRUE(v.properlyInitialized());

    EXPECT_DEATH(Vaccin("Naam", 1, -10, 1, 2, 20),
                 "Interval can't be negative");

    EXPECT_DEATH(Vaccin("", 1, 10, 1, 2, 20),
                 "type can't be empty");

    EXPECT_DEATH(Vaccin("Naam", 1, 10, -1, 2, 20),
                 "Transport can't be negative");

    EXPECT_DEATH(Vaccin("Naam", 1, -10, 1, -2, 20),
                 "Interval can't be negative");
}

// Test VaccinationCenter::vaccinType
TEST_F(VaccinDistributorDomainTests, VaccinType) {

    std::string testName = "VaccinType";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      75000, 5500);
    EXPECT_TRUE(center->properlyInitialized());
    EXPECT_EQ("Flanders Expo", center->getName());
    EXPECT_EQ("Maaltekouter 1, 9051 Gent", center->getAddress());
    EXPECT_EQ(75000, center->getPopulation());
    EXPECT_EQ(5500, center->getCapacity());
    EXPECT_EQ(0, center->getVaccins());
    EXPECT_EQ(0, center->getVaccinated());

    Vaccin* vaccin = new Vaccin("Pfizer", 45000, 12, 2000, 4, -15);

    EXPECT_TRUE(vaccin->properlyInitialized());
    EXPECT_EQ("Pfizer", vaccin->getType());
    EXPECT_EQ(45000, vaccin->getDelivery());
    EXPECT_EQ(12, vaccin->getInterval());
    EXPECT_EQ(2000, vaccin->getTransport());
    EXPECT_EQ(4, vaccin->getRenewal());
    EXPECT_EQ(-15, vaccin->getTemperature());
    EXPECT_TRUE(vaccin->checkUnderZero());
    EXPECT_EQ(45000, vaccin->getVaccin());

    EXPECT_DEATH(center->addVaccins(12000, vaccin), "Amount of vaccins must not exceed capacity of Center");
    center->addVaccins(7500, vaccin);

    EXPECT_EQ(7500, center->getVaccins());
    EXPECT_EQ(5500, center->calculateVaccinationAmount());
    EXPECT_EQ("Pfizer", center->getVaccin(true).begin()->first);
    EXPECT_TRUE(center->getVaccin(true).begin()->second->properlyInitialized());
    EXPECT_EQ("Pfizer", center->getVaccin(true).begin()->second->getVaccinType());
    EXPECT_EQ(-15, center->getVaccin(true).begin()->second->getVaccinTemperature());
    EXPECT_EQ(4, center->getVaccin(true).begin()->second->getVaccinRenewal());
    EXPECT_EQ(7500, center->getVaccin(true).begin()->second->getVaccinAmount());
    EXPECT_TRUE(center->getVaccin(true).begin()->second->getTracker().empty());
    EXPECT_TRUE(center->getVaccin(true).begin()->second->isRenewal());
    EXPECT_EQ(0, center->getVaccin(true).begin()->second->totalFirstVaccination());

    Vaccin* vaccin1 = new Vaccin("Atrazenica", 20000, 12, 4000, 0, 11);

    EXPECT_TRUE(vaccin1->properlyInitialized());
    EXPECT_EQ("Atrazenica", vaccin1->getType());
    EXPECT_EQ(20000, vaccin1->getDelivery());
    EXPECT_EQ(12, vaccin1->getInterval());
    EXPECT_EQ(4000, vaccin1->getTransport());
    EXPECT_EQ(0, vaccin1->getRenewal());
    EXPECT_EQ(11, vaccin1->getTemperature());
    EXPECT_FALSE(vaccin1->checkUnderZero());
    EXPECT_EQ(20000, vaccin1->getVaccin());

    center->addVaccins(0, vaccin1);

    center->print(ostream);
    center->printGraphical(ostream);

    EXPECT_TRUE(center->getVaccin(false).size() == 1);
    EXPECT_TRUE(center->getVaccin(true).size() == 1);

    center->addVaccins(2000, vaccin1);

    EXPECT_TRUE(center->getVaccin(false).size() == 1);
    EXPECT_TRUE(center->getVaccin(true).size() == 1);

    EXPECT_EQ(9500, center->getVaccins());
    EXPECT_EQ(5500, center->calculateVaccinationAmount());
    EXPECT_EQ("Atrazenica", center->getVaccin(false).begin()->first);
    EXPECT_TRUE(center->getVaccin(false).begin()->second->properlyInitialized());
    EXPECT_EQ("Atrazenica", center->getVaccin(false).begin()->second->getVaccinType());
    EXPECT_EQ(11, center->getVaccin(false).begin()->second->getVaccinTemperature());
    EXPECT_EQ(0, center->getVaccin(false).begin()->second->getVaccinRenewal());
    EXPECT_EQ(2000, center->getVaccin(false).begin()->second->getVaccinAmount());
    EXPECT_FALSE(center->getVaccin(false).begin()->second->isRenewal());

    EXPECT_EQ(1500, center->getOpenVaccinStorage(vaccin1));

    center->vaccinateCenter(ostream);
    EXPECT_EQ(0, center->getVaccinated());
    EXPECT_EQ(4000, center->getVaccins());

    center->print(ostream);
    center->printGraphical(ostream);

    EXPECT_EQ(2000, center->getVaccin(true).begin()->second->getVaccinAmount());
    EXPECT_FALSE(center->getVaccin(true).begin()->second->getTracker().empty());
    EXPECT_EQ(-4, center->getVaccin(true).begin()->second->getTracker().begin()->first);
    EXPECT_EQ(5500, center->getVaccin(true).begin()->second->getTracker().begin()->second);
    EXPECT_EQ(5500, center->getVaccin(true).begin()->second->totalFirstVaccination());

    for (int i = 0; i != 4; i++) {
        center->updateRenewal();
    }

    EXPECT_EQ("Pfizer", center->requiredAmountVaccinType().begin()->first);
    EXPECT_EQ(3500, center->requiredAmountVaccin(center->getVaccin(true).begin()->second));

    center->vaccinateCenter(ostream);

    EXPECT_EQ(0, center->getVaccin(true).begin()->second->getVaccinAmount());
    EXPECT_FALSE(center->getVaccin(true).begin()->second->getTracker().empty());
    EXPECT_EQ(0, center->getVaccin(true).begin()->second->getTracker().begin()->first);
    EXPECT_EQ(3500, center->getVaccin(true).begin()->second->getTracker().begin()->second);
    EXPECT_EQ(3500, center->getVaccin(true).begin()->second->totalFirstVaccination());

    EXPECT_EQ(0, center->getVaccins());
    center->printGraphical(ostream);

    ostream.close();
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Test addVaccin()
TEST_F(VaccinDistributorDomainTests, AddVaccin) {

    Hub hub = Hub();

    EXPECT_TRUE(hub.getVaccins().empty());
    EXPECT_TRUE(hub.getCentra().empty());
    EXPECT_TRUE(hub.properlyInitialized());

    Vaccin* vaccin = new Vaccin("Pfizer", 45000,
                                12, 2000, 4, -15);

    EXPECT_TRUE(vaccin->properlyInitialized());
    EXPECT_EQ("Pfizer", vaccin->getType());
    EXPECT_EQ(45000, vaccin->getDelivery());
    EXPECT_EQ(12, vaccin->getInterval());
    EXPECT_EQ(2000, vaccin->getTransport());
    EXPECT_EQ(4, vaccin->getRenewal());
    EXPECT_EQ(-15, vaccin->getTemperature());
    EXPECT_TRUE(vaccin->checkUnderZero());
    EXPECT_EQ(45000, vaccin->getVaccin());
    vaccin->updateVaccins();
    EXPECT_EQ(90000, vaccin->getVaccin());
    EXPECT_DEATH(vaccin->updateVaccinsTransport(45000), "Wrong transport amount, Cargo amount must be a int");
    vaccin->updateVaccinsTransport(8000);
    EXPECT_EQ(82000, vaccin->getVaccin());
    EXPECT_TRUE(vaccin->checkUnderZero());

    hub.addVaccin(vaccin);
    EXPECT_FALSE(hub.getVaccins().empty());
    EXPECT_EQ("Pfizer", hub.getVaccins().find("Pfizer")->first);
    EXPECT_EQ(vaccin, hub.getVaccins().find("Pfizer")->second);
}

// Test Vaccin constructor with wrong values
TEST_F(VaccinDistributorDomainTests, VaccinWrongValues) {

    EXPECT_DEATH(Vaccin* vaccin = new Vaccin("", 45000, 12, 2000, 4, -15), "type can't be empty");
    EXPECT_DEATH(Vaccin* vaccin = new Vaccin("Pfizer", -45000, 12, 2000, 4, -15), "Delivery can't be negative");
    EXPECT_DEATH(Vaccin* vaccin = new Vaccin("Pfizer", 45000, -12, 2000, 4, -15), "Interval can't be negative");
    EXPECT_DEATH(Vaccin* vaccin = new Vaccin("Pfizer", 45000, 12, -2000, 4, -15), "Transport can't be negative");
    EXPECT_DEATH(Vaccin* vaccin = new Vaccin("Pfizer", 45000, 12, 2000, -4, -15), "Renewal can't be negative");
}

// Double Vaccin addVaccin()
TEST_F(VaccinDistributorDomainTests, DoubleAddVaccin) {

    Hub hub = Hub();

    EXPECT_TRUE(hub.getVaccins().empty());
    EXPECT_TRUE(hub.getCentra().empty());
    EXPECT_TRUE(hub.properlyInitialized());

    Vaccin* vaccin = new Vaccin("Pfizer", 45000,
                                12, 2000, 4, -15);

    EXPECT_TRUE(vaccin->properlyInitialized());
    EXPECT_EQ("Pfizer", vaccin->getType());
    EXPECT_EQ(45000, vaccin->getDelivery());
    EXPECT_EQ(12, vaccin->getInterval());
    EXPECT_EQ(2000, vaccin->getTransport());
    EXPECT_EQ(4, vaccin->getRenewal());
    EXPECT_EQ(-15, vaccin->getTemperature());
    EXPECT_TRUE(vaccin->checkUnderZero());
    EXPECT_EQ(45000, vaccin->getVaccin());
    vaccin->updateVaccins();
    EXPECT_EQ(90000, vaccin->getVaccin());
    EXPECT_DEATH(vaccin->updateVaccinsTransport(45000), "Wrong transport amount, Cargo amount must be a int");
    vaccin->updateVaccinsTransport(8000);
    EXPECT_EQ(82000, vaccin->getVaccin());
    EXPECT_TRUE(vaccin->checkUnderZero());

    hub.addVaccin(vaccin);
    EXPECT_FALSE(hub.getVaccins().empty());
    EXPECT_EQ("Pfizer", hub.getVaccins().find("Pfizer")->first);
    EXPECT_EQ(vaccin, hub.getVaccins().find("Pfizer")->second);

    Vaccin* vaccin1 = new Vaccin("Pfizer", 20000, 12, 1000, 2, 82);
    EXPECT_FALSE(vaccin1->checkUnderZero());
    EXPECT_DEATH(hub.addVaccin(vaccin1), "Vaccin can't yet exist in Hub");
}

// Test distributeVaccinsFair()
TEST_F(VaccinDistributorDomainTests, DistributeVaccinsFair) {

    std::string testName = "DistributeVaccinsFair";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    Hub hub = Hub();
    EXPECT_TRUE(hub.properlyInitialized());

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      20000, 10000);
    VaccinationCenter *center1 = new VaccinationCenter("Park Spoor Oost", "Maaltekouter 1, 9051 Gent",
                                                      20000, 10000);
    EXPECT_TRUE(center->properlyInitialized());
    EXPECT_TRUE(center1->properlyInitialized());
    hub.addCenter(center->getName(), center);
    hub.addCenter(center1->getName(), center1);

    Vaccin* vaccin = new Vaccin("Pfizer", 15300, 1, 500, 0,  20);
    EXPECT_TRUE(vaccin->properlyInitialized());
    hub.addVaccin(vaccin);


    hub.distributeVaccinsFair(vaccin, 1, ostream);
    EXPECT_EQ(300, hub.getAmountVaccin());
    EXPECT_EQ(7500, center->getVaccins());
    EXPECT_EQ(7500, center1->getVaccins());
    EXPECT_EQ(0, center->getVaccinated());
    EXPECT_EQ(0, center1->getVaccinated());
    center->vaccinateCenter(ostream);
    center1->vaccinateCenter(ostream);

    hub.updateVaccins();
    hub.distributeVaccinsFair(vaccin, 0, ostream);
    EXPECT_EQ(100, hub.getAmountVaccin());
    EXPECT_EQ(7500, center->getVaccins());
    EXPECT_EQ(8000, center1->getVaccins());
    EXPECT_EQ(7500, center->getVaccinated());
    EXPECT_EQ(7500, center1->getVaccinated());
    center->vaccinateCenter(ostream);
    center1->vaccinateCenter(ostream);

    hub.updateVaccins();
    hub.distributeVaccinsFair(vaccin, 0, ostream);
    EXPECT_EQ(5900, hub.getAmountVaccin());
    EXPECT_EQ(5000, center->getVaccins());
    EXPECT_EQ(4500, center1->getVaccins());
    EXPECT_EQ(15000, center->getVaccinated());
    EXPECT_EQ(15500, center1->getVaccinated());
    center->vaccinateCenter(ostream);
    center1->vaccinateCenter(ostream);

    hub.print(ostream);
    center->print(ostream);
    center1->print(ostream);

    delete center;
    delete center1;

    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileExists("tests/domainTests/expectedOutput/expectedDistributeVaccinsFair.txt"));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare) || FileCompare(fileName,
                "tests/domainTests/expectedOutput/expectedDistributeVaccinsFair2.txt"));
}

// Test transportNonExistingVaccin() with not-existing Vaccin in Hub
TEST_F(VaccinDistributorDomainTests, TransportNonExistingVaccin) {

    Hub hub = Hub();
    EXPECT_TRUE(hub.properlyInitialized());
    std::ofstream ostream;

    Vaccin* vaccin = new Vaccin("Pfizer", 15300, 1, 500, 0,  20);
    EXPECT_TRUE(vaccin->properlyInitialized());

    EXPECT_DEATH(hub.distributeVaccinsFair(vaccin, 0, ostream), "Given vaccin must exist");
    delete vaccin;
}

// Test Hub with multiple centra
TEST_F(VaccinDistributorDomainTests, HubMultipleCenter) {

    std::string testName = "hubMultipleCenter";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    Hub hub = Hub();
    EXPECT_TRUE(hub.properlyInitialized());

    VaccinationCenter *center1 = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      20000, 10000);
    EXPECT_TRUE(center1->properlyInitialized());
    hub.addCenter(center1->getName(), center1);

    VaccinationCenter* center2 = new VaccinationCenter("Park Spoor Oost", "Noordersingel 28/30, 2140 Antwerpen"
            , 54321, 7500);
    EXPECT_TRUE(center2->properlyInitialized());
    hub.addCenter(center2->getName(), center2);

    VaccinationCenter* center3 = new VaccinationCenter("Park Spoor Noord", "Oostersingel 28/30, 2140 Antwerpen"
            , 10000, 4500);

    EXPECT_TRUE(center3->properlyInitialized());
    hub.addCenter(center3->getName(), center3);

    hub.print(ostream);
    delete center1;
    delete center2;
    delete center3;
    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Test constructor of VaccinationCenter object and addVaccin()
TEST_F(VaccinDistributorDomainTests, ConstructorCenter) {

    VaccinationCenter center = VaccinationCenter("Park Spoor Oost", "Noordersingel 28/30, 2140 Antwerpen"
                                                 , 54321, 7500);

    EXPECT_TRUE(center.properlyInitialized());
    EXPECT_EQ("Park Spoor Oost", center.getName());
    EXPECT_EQ("Noordersingel 28/30, 2140 Antwerpen", center.getAddress());
    EXPECT_EQ(54321, center.getPopulation());
    EXPECT_EQ(7500, center.getCapacity());
    EXPECT_EQ(0, center.getVaccins());
    EXPECT_EQ(0, center.getVaccinated());
}

// Test addVaccin()
TEST_F(VaccinDistributorDomainTests, AddVaccinDeath) {

    VaccinationCenter center = VaccinationCenter("Park Spoor Oost", "Noordersingel 28/30, 2140 Antwerpen"
            , 54321, 7500);
    Vaccin* vaccin = new Vaccin("Pfizer", 15300, 1, 500, 0,  20);
    EXPECT_TRUE(vaccin->properlyInitialized());
    EXPECT_TRUE(center.properlyInitialized());
    EXPECT_DEATH(center.addVaccins(15001,vaccin), "Amount of vaccins must not exceed capacity of Center");
    center.addVaccins(15000,vaccin);
}

// Tests calculateVaccinationAmount() and vaccinateCenter()
TEST_F(VaccinDistributorDomainTests, CalculateVaccin) {

    std::string testName = "CalculateVaccin";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    Vaccin* vaccin = new Vaccin("Pfizer", 15300, 1, 500, 0,  20);

    VaccinationCenter center = VaccinationCenter("Park Spoor Oost", "Noordersingel 28/30, 2140 Antwerpen"
            , 54321, 70000);

    EXPECT_TRUE(center.properlyInitialized());
    EXPECT_EQ("Park Spoor Oost", center.getName());
    EXPECT_EQ("Noordersingel 28/30, 2140 Antwerpen", center.getAddress());
    EXPECT_EQ(54321, center.getPopulation());
    EXPECT_EQ(70000, center.getCapacity());
    EXPECT_EQ(0, center.getVaccins());
    EXPECT_EQ(0, center.getVaccinated());
    EXPECT_EQ(0, center.calculateVaccinationAmount());

    EXPECT_DEATH(center.addVaccins(140001,vaccin), "Amount of vaccins must not exceed capacity of Center");
    center.addVaccins(4500, vaccin);
    EXPECT_EQ(4500, center.getVaccins());
    EXPECT_EQ(4500, center.calculateVaccinationAmount());
    center.addVaccins(50000, vaccin);
    EXPECT_EQ(54500, center.getVaccins());
    EXPECT_FALSE(54500 == center.calculateVaccinationAmount());
    EXPECT_EQ(54321, center.calculateVaccinationAmount());
    EXPECT_EQ(0, center.getVaccinated());
    center.vaccinateCenter(ostream);
    EXPECT_EQ(54321, center.getVaccinated());
    center.print(ostream);
    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Tests calculateVaccinationAmount() and vaccinateCenter()
TEST_F(VaccinDistributorDomainTests, VaccinateCenter) {

    Vaccin* vaccin = new Vaccin("Pfizer", 15300, 1, 500, 0,  20);
    std::string testName = "VaccinateCenter";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    VaccinationCenter center = VaccinationCenter("Park Spoor Oost", "Noordersingel 28/30, 2140 Antwerpen"
            , 54321, 15000);

    EXPECT_TRUE(center.properlyInitialized());
    EXPECT_EQ("Park Spoor Oost", center.getName());
    EXPECT_EQ("Noordersingel 28/30, 2140 Antwerpen", center.getAddress());
    EXPECT_EQ(54321, center.getPopulation());
    EXPECT_EQ(15000, center.getCapacity());
    EXPECT_EQ(0, center.getVaccins());
    EXPECT_EQ(0, center.getVaccinated());
    EXPECT_EQ(0, center.calculateVaccinationAmount());

    center.addVaccins(30000, vaccin);
    EXPECT_EQ(30000, center.getVaccins());
    center.vaccinateCenter(ostream);
    EXPECT_EQ(15000, center.getVaccinated());
    EXPECT_EQ(15000, center.getVaccins());
    center.addVaccins(15000, vaccin);
    EXPECT_EQ(30000, center.getVaccins());
    center.print(ostream);
    center.vaccinateCenter(ostream);
    EXPECT_EQ(15000, center.getVaccins());
    center.addVaccins(15000, vaccin);
    center.vaccinateCenter(ostream);
    center.print(ostream);
    center.vaccinateCenter(ostream);
    EXPECT_EQ(54321, center.getVaccinated());
    EXPECT_EQ(5679, center.getVaccins());

    center.print(ostream);
    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Test createn of Hub with centra and vaccins
TEST_F(VaccinDistributorDomainTests, Create) {
    //Simulation test
    Hub* h = new Hub;
    Vaccin* vaccin = new Vaccin("Vaccin 1", 15300, 1, 500, 0,  -20);
    Vaccin* vaccin1 = new Vaccin("Vaccin 2", 30000, 2, 1000, 0,  -80);
    Vaccin* vaccin2 = new Vaccin("Vaccin 3", 2000, 1, 500, 12,  20);
    VaccinationCenter* center = new VaccinationCenter("Center 1", "Locatie 1", 54321, 15000);
    VaccinationCenter* center1 = new VaccinationCenter("Center 2", "Locatie 2", 30000, 20000);
    VaccinationCenter* center2 = new VaccinationCenter("Center 3", "Locatie 3", 20000, 19000);

    EXPECT_TRUE(h->properlyInitialized());
    EXPECT_TRUE(vaccin->properlyInitialized());
    EXPECT_TRUE(vaccin1->properlyInitialized());
    EXPECT_TRUE(vaccin2->properlyInitialized());
    EXPECT_TRUE(center->properlyInitialized());
    EXPECT_TRUE(center1->properlyInitialized());
    EXPECT_TRUE(center2->properlyInitialized());

    EXPECT_FALSE(h->containsVaccinationCenter(center->getName()));
    EXPECT_FALSE(h->containsVaccinationCenter(center->getName()));
    EXPECT_FALSE(h->containsVaccinationCenter(center->getName()));
    h->addCenter(center->getName(), center);
    h->addCenter(center1->getName(), center1);
    h->addCenter(center2->getName(), center2);
    EXPECT_TRUE(h->containsVaccinationCenter(center->getName()));
    EXPECT_TRUE(h->containsVaccinationCenter(center->getName()));
    EXPECT_TRUE(h->containsVaccinationCenter(center->getName()));
    EXPECT_TRUE(h->getCentra().size() == 3);

    EXPECT_FALSE(h->containsVaccin(vaccin));
    EXPECT_FALSE(h->containsVaccin(vaccin1));
    EXPECT_FALSE(h->containsVaccin(vaccin2));
    h->addVaccin(vaccin);
    h->addVaccin(vaccin1);
    h->addVaccin(vaccin2);
    EXPECT_TRUE(h->containsVaccin(vaccin));
    EXPECT_TRUE(h->containsVaccin(vaccin1));
    EXPECT_TRUE(h->containsVaccin(vaccin2));
    EXPECT_TRUE(h->getVaccins().size() == 3);
}
