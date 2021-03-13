/**
 * @file VdDomainTests.cpp
 * @brief This file is used to run tests for the Hub class
 * @author Pablo Deputter
 * @date 11/03/2012
 */

#include <fstream>
#include <iostream>
#include "gtest/gtest.h"
#include "Simulation.h"

// TODO expect death (wat?)

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
TEST_F(VaccinDistributorDomainTests, defaultConstructorSimulation) {

    Simulation simulation = Simulation();

    EXPECT_TRUE(simulation.properlyInitialized());
    EXPECT_FALSE(!simulation.properlyInitialized());
    EXPECT_EQ(0, simulation.getIter());
    EXPECT_EQ(NULL, simulation.getFhub());
    EXPECT_TRUE(simulation.getFcentra().empty());
}

// Test iterator of Simulation object
TEST_F(VaccinDistributorDomainTests, iterator) {

    Simulation simulation = Simulation();

    EXPECT_TRUE(simulation.properlyInitialized());
    EXPECT_EQ(0, simulation.getIter());

    simulation.increaseIterator();
    EXPECT_EQ(1, simulation.getIter());
}

// Test constructor of Hub object
TEST_F(VaccinDistributorDomainTests, constructorHub) {

    Hub hub = Hub(400000, 6, 40000);

    EXPECT_TRUE(hub.properlyInitialized());
    EXPECT_FALSE(!hub.properlyInitialized());
    EXPECT_EQ(400000, hub.getDelivery());
    EXPECT_EQ(6, hub.getInterval());
    EXPECT_EQ(40000, hub.getTransport());
    EXPECT_EQ(400000, hub.getVaccin());
    EXPECT_TRUE(hub.getCentra().empty());
}

// Test addCenter()
TEST_F(VaccinDistributorDomainTests, addCenter) {

    Hub hub = Hub(10000, 6, 2500);

    EXPECT_TRUE(hub.properlyInitialized());
    EXPECT_EQ(10000, hub.getDelivery());
    EXPECT_EQ(10000, hub.getDelivery());
    EXPECT_EQ(6, hub.getInterval());
    EXPECT_EQ(2500, hub.getTransport());
    EXPECT_EQ(10000, hub.getVaccin());
    EXPECT_TRUE(hub.getCentra().empty());

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

    delete center;
}

// Double VaccinationCenter
TEST_F(VaccinDistributorDomainTests, doubleVaccinationCenter) {

    Hub hub = Hub(10000, 6, 2500);
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

    delete center1;
    delete center2;
}

// test calculateTransport()
TEST_F(VaccinDistributorDomainTests, calculateTransport) {

    std::string testName = "calculateTransport";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    Hub hub = Hub(10000, 6, 2500);
    EXPECT_TRUE(hub.properlyInitialized());

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      15000, 5500);
    EXPECT_TRUE(center->properlyInitialized());
    hub.addCenter("Flanders Expo", center);

    EXPECT_EQ(7500, hub.calculateTransport(center));
    center->addVaccins(7500);
    center->vaccinateCenter(ostream);
    EXPECT_EQ(5000, hub.calculateTransport(center));
    center->addVaccins(5000);
    center->vaccinateCenter(ostream);
    EXPECT_EQ(5000, hub.calculateTransport(center));
    center->addVaccins(5000);
    center->vaccinateCenter(ostream);
    center->print(ostream);
    delete center;
    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Test transportVaccin()
TEST_F(VaccinDistributorDomainTests, transportVaccin) {

    std::string testName = "transportVaccin";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    Hub hub = Hub(30000, 6, 7000);
    EXPECT_TRUE(hub.properlyInitialized());

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      20000, 10000);
    EXPECT_TRUE(center->properlyInitialized());
    hub.addCenter(center->getName(), center);

    hub.transportVaccin("Flanders Expo", ostream);
    EXPECT_EQ(16000, hub.getVaccin());
    EXPECT_EQ(14000, center->getVaccins());
    EXPECT_EQ(0, center->getVaccinated());
    center->vaccinateCenter(ostream);
    hub.transportVaccin("Flanders Expo", ostream);
    EXPECT_EQ(9000, hub.getVaccin());
    EXPECT_EQ(11000, center->getVaccins());
    EXPECT_EQ(10000, center->getVaccinated());
    center->vaccinateCenter(ostream);
    hub.transportVaccin("Flanders Expo", ostream);
    EXPECT_EQ(2000, hub.getVaccin());
    EXPECT_EQ(8000, center->getVaccins());
    EXPECT_EQ(20000, center->getVaccinated());
    center->vaccinateCenter(ostream);
    hub.print(ostream);
    center->print(ostream);

    delete center;
    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
}

// Test transportVaccin() with not-existing Center name
TEST_F(VaccinDistributorDomainTests, transportVaccinDeath) {

    Hub hub = Hub(30000, 6, 7000);
    EXPECT_TRUE(hub.properlyInitialized());
    std::ofstream ostream;

    EXPECT_DEATH(hub.transportVaccin("Flanders Expo", ostream), "Given centerName must exist");

    VaccinationCenter *center = new VaccinationCenter("Flanders Expo", "Maaltekouter 1, 9051 Gent",
                                                      20000, 10000);
    EXPECT_TRUE(center->properlyInitialized());
    hub.addCenter("Flanders Expo", center);
    EXPECT_DEATH(hub.transportVaccin("Flanders expo", ostream), "Given centerName must exist");
    hub.transportVaccin("Flanders Expo", ostream);
    delete center;
}

// Test Hub with multiple centra
TEST_F(VaccinDistributorDomainTests, hubMultipleCenter) {

    std::string testName = "hubMultipleCenter";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    Hub hub = Hub(42000, 6, 1000);
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

    // TODO wat?

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

// Test default constructor of VaccinationCenter object
TEST_F(VaccinDistributorDomainTests, defaultConstructorCenter) {

    VaccinationCenter center = VaccinationCenter();

    EXPECT_TRUE(center.properlyInitialized());
    EXPECT_EQ("", center.getName());
    EXPECT_EQ("", center.getAddress());
    EXPECT_EQ(0, center.getPopulation());
    EXPECT_EQ(0, center.getCapacity());
    EXPECT_EQ(0, center.getVaccins());
    EXPECT_EQ(0, center.getVaccinated());
}

// Test non-default constructor of VaccinationCenter object and addVaccin()
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

    center.addVaccins(4500);

    EXPECT_FALSE(0 == center.getVaccins());
    EXPECT_EQ(4500, center.getVaccins());
}

// Test addVaccin()
TEST_F(VaccinDistributorDomainTests, addVaccinDeath) {

    VaccinationCenter center = VaccinationCenter("Park Spoor Oost", "Noordersingel 28/30, 2140 Antwerpen"
            , 54321, 7500);
    EXPECT_TRUE(center.properlyInitialized());
    EXPECT_DEATH(center.addVaccins(15001), "Amount of vaccins must not exceed capacity of Center");
    center.addVaccins(15000);
}

// Tests calculateVaccinationAmount() and vaccinateCenter()
TEST_F(VaccinDistributorDomainTests, calculateVaccin) {

    std::string testName = "calculateVaccin";
    std::ofstream ostream;
    std::string fileName = "tests/domainTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/domainTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

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

    EXPECT_DEATH(center.addVaccins(140001), "Amount of vaccins must not exceed capacity of Center");
    center.addVaccins(4500);
    EXPECT_EQ(4500, center.getVaccins());
    EXPECT_EQ(4500, center.calculateVaccinationAmount());
    center.addVaccins(50000);
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
TEST_F(VaccinDistributorDomainTests, vaccinateCenter) {

    std::string testName = "vaccinateCenter";
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

    center.addVaccins(30000);
    EXPECT_EQ(30000, center.getVaccins());
    center.vaccinateCenter(ostream);
    EXPECT_EQ(15000, center.getVaccinated());
    EXPECT_EQ(15000, center.getVaccins());
    center.addVaccins(15000);
    EXPECT_EQ(30000, center.getVaccins());
    center.print(ostream);
    center.vaccinateCenter(ostream);
    EXPECT_EQ(15000, center.getVaccins());
    center.addVaccins(15000);
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