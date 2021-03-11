/**
 * @file VdSimulateTests.cpp
 * @brief This file is used to all the tests of the Simulate class - input, output - based off Serge Demeyer tests
 * @author Pablo Deputter
 * @date 11/03/2012
 */

#include "gtest/gtest.h"
#include <fstream>
#include "Simulation.h"

class VaccinSimulateTests : public::testing::Test {

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
    Simulation s;
};

TEST_F(VaccinSimulateTests, noHappyDays) {

    std::ofstream ostream;

    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_DEATH(s.importXmlFile("tests/inputTests/noHappyDays.xml"),
                 "The file that needs to be read must not be empty");
    EXPECT_DEATH(s.importXmlFile("nonexistent.xml"), "The file that needs to be read must exist");
    EXPECT_DEATH(s.checkConnections(), "Hub must exist");
    EXPECT_FALSE(s.checkCentra());
    EXPECT_FALSE(s.checkHub());
    EXPECT_FALSE(s.checkSimulation());
    EXPECT_DEATH(s.exportFile("tests/inputTests/file1.txt"),
                 "The simulation must be valid/consistent");
    EXPECT_TRUE(s.getFcentra().empty());
    EXPECT_DEATH(s.automaticSimulation(1,1,2012,ostream),
                 "The simulation must be valid/consistent");
    EXPECT_DEATH(s.simulateVaccination(ostream), "The simulation must be valid/consistent");
    EXPECT_DEATH(s.simulateTransport(ostream), "The simulation must be valid/consistent");
}

TEST_F(VaccinSimulateTests, HappyDays) {

    std::string testName = "happyDays";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/happyDays.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());
    s.simulateTransport(ostream);
    ostream << std::endl;
    s.simulateVaccination(ostream);
    ostream << std::endl;





    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));

}

