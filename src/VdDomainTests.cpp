/**
 * @file VdDomainTests.cpp
 * @brief This file is used to run tests for the Hub class
 * @author Pablo Deputter
 * @date 11/03/2012
 */

#include <fstream>
#include <iostream>
#include "gtest/gtest.h"
#include "../../PSE 2/src/Simulation.h"

class VaccinDistributorDomainTests : public::testing::Test {

protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
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


TEST_F(VaccinDistributorDomainTests, getCentra) {

    Simulation simulation = Simulation();

}

// Test constructor of Hub object
TEST_F(VaccinDistributorDomainTests, constructorHub) {

    Hub hub = Hub(400000, 6, 40000);

    EXPECT_TRUE(hub.properlyInitialized());
    EXPECT_FALSE(!hub.properlyInitialized());
    EXPECT_EQ(400000, hub.getFdelivery());
    EXPECT_EQ(6, hub.getFinterval());
    EXPECT_EQ(40000, hub.getFtransport());
    EXPECT_EQ(400000, hub.getFvaccin());
    EXPECT_TRUE(hub.getFcentra().empty());
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

