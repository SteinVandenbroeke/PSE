/**
 * @file VdSimulateTests.cpp
 * @brief This file is used to all the tests of the Simulate class - input, output - based off Serge Demeyer tests
 * @author Pablo Deputter
 * @date 11/03/2012
 */

#include "gtest/gtest.h"
#include <fstream>
#include "Simulation.h"

class VaccinSimulationTests : public::testing::Test {

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
        ASSERT_TRUE(DirectoryExists("tests/inputTests"));
        ASSERT_TRUE(DirectoryExists("tests/outputTests/expectedOutput"));
        ASSERT_TRUE(DirectoryExists("tests/outputTests/generatedOutput"));
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

    // Declares the variables your tests want to use.
    Simulation s;
};

// Test simulation with an "empty" Simulation object
TEST_F(VaccinSimulationTests, emptySimulation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptySimulation.xml"));
    std::ofstream ostream;

    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_DEATH(s.importXmlFile("tests/inputTests/emptySimulation.xml"),
                 "The file that needs to be read must not be empty");
    EXPECT_DEATH(s.importXmlFile("nonexistent.xml"), "The file that needs to be read must exist");
    EXPECT_DEATH(s.checkConnections(), "Hub must exist");
    EXPECT_FALSE(s.checkCentra());
    EXPECT_FALSE(s.checkHub());
    EXPECT_FALSE(s.checkSimulation());
    EXPECT_DEATH(s.exportFile("tests/inputTests/file1.txt"),
                 "The simulation must be valid/consistent");
    EXPECT_TRUE(s.getFcentra().empty());
    EXPECT_DEATH(s.automaticSimulation(5, ostream),
                 "The simulation must be valid/consistent");
    EXPECT_DEATH(s.simulateVaccination(ostream), "The simulation must be valid/consistent");
    EXPECT_DEATH(s.simulateTransport(ostream), "The simulation must be valid/consistent");
}

// Test automatic simulation with no equal vaccins as delivery on day zero
TEST_F(VaccinSimulationTests, noHappyDays) {

    ASSERT_TRUE(FileExists("tests/inputTests/noHappyDays.xml"));

    std::string testName = "noHappyDays";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/noHappyDays.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());
    s.simulateTransport(ostream);
    ostream << std::endl;
    s.simulateVaccination(ostream);
    EXPECT_TRUE(s.checkSimulation());

    EXPECT_DEATH(s.automaticSimulation(5, ostream),
                 "Hub must have equal amount of vaccins as delivery on day zero");

    s.exportFile("tests/outputTests/generatedOutput/generated" + testName + "_" + ".txt");
    ostream.close();

    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists("tests/outputTests/generatedOutput/generated" + testName + "_" + ".txt"));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
    EXPECT_TRUE(FileCompare("tests/outputTests/expectedOutput/expected" + testName + "_" + ".txt",
                            "tests/outputTests/generatedOutput/generated" + testName + "_" + ".txt"));
}

TEST_F(VaccinSimulationTests, wrongClosingXml) {
    ASSERT_TRUE(FileExists("tests/inputTests/wrongClosingTagXml.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/wrongLayoutXml.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/containsWrongTagsXml.xml"));

    EXPECT_THROW(s.importXmlFile("tests/inputTests/wrongClosingTagXml.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());

    EXPECT_THROW(s.importXmlFile("tests/inputTests/wrongLayoutXml.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());

    EXPECT_NO_THROW(s.importXmlFile("tests/inputTests/containsWrongTagsXml.xml"));
    EXPECT_TRUE(s.checkSimulation());
}

TEST_F(VaccinSimulationTests, wrongCentraInformation) {
    ASSERT_TRUE(FileExists("tests/inputTests/wrongCentra.xml"));

    ostringstream errorStream;
    s.importXmlFile("tests/inputTests/wrongCentra.xml", errorStream);
    EXPECT_EQ("Element not found: inwoners\nHub contains an non existing or wrong vactination centrum: 'Park Spoor Oost'\n"
              , errorStream.str());
    EXPECT_TRUE(s.checkSimulation());
}

TEST_F(VaccinSimulationTests, wrongHubInformation) {
    ASSERT_TRUE(FileExists("tests/inputTests/wrongHub.xml"));

    ostringstream errorStream;

    s.importXmlFile("tests/inputTests/wrongHub.xml", errorStream);
    EXPECT_EQ("Empty centra name!\n"
    , errorStream.str());
    EXPECT_TRUE(s.checkSimulation());
}

TEST_F(VaccinSimulationTests, brokenHubInformation) {
    ASSERT_TRUE(FileExists("tests/inputTests/brokenHub.xml"));

    EXPECT_THROW(s.importXmlFile("tests/inputTests/brokenHub.xml"),Exception);
    EXPECT_FALSE(s.checkSimulation());
}

TEST_F(VaccinSimulationTests, noHub) {

    ASSERT_TRUE(FileExists("tests/inputTests/noHub.xml"));

    EXPECT_THROW(s.importXmlFile("tests/inputTests/noHub.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());
}

TEST_F(VaccinSimulationTests, noCentra) {

    ASSERT_TRUE(FileExists("tests/inputTests/noCentra.xml"));

    EXPECT_THROW(s.importXmlFile("tests/inputTests/noCentra.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());
}

TEST_F(VaccinSimulationTests, wrongFile) {

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/noCentra.txt"),
                 "The file that needs to be read must exist");
    EXPECT_FALSE(s.checkSimulation());
}

TEST_F(VaccinSimulationTests, emptyFile) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyFile.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/emptyFile.xml"),
                 "The file that needs to be read must not be empty");
    EXPECT_FALSE(s.checkSimulation());
}

TEST_F(VaccinSimulationTests, intenseInterval) {

    ASSERT_TRUE(FileExists("tests/inputTests/intenseInterval.xml"));

    std::string testName = "intenseInterval";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/intenseInterval.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());

    EXPECT_EQ(4, s.getFcentra().size());
    EXPECT_TRUE(NULL != s.getFhub());
    EXPECT_EQ(93000, s.getFhub()->getVaccin());
    EXPECT_EQ("Park Spoor Oost", s.getFcentra().find("Park Spoor Oost")->first);
    EXPECT_EQ("AED Studios", s.getFcentra().find("AED Studios")->first);
    EXPECT_EQ("De Zoerla", s.getFcentra().find("De Zoerla")->first);
    EXPECT_EQ("Flanders Expo", s.getFcentra().find("Flanders Expo")->first);
    s.automaticSimulation(18, ostream);
    std::string exportFileName = "tests/outputTests/generatedOutput/generatedintenseInterval_.txt";
    s.exportFile(exportFileName);

    ostream.close();
    EXPECT_TRUE(FileExists(exportFileName));
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
    EXPECT_TRUE(FileCompare(exportFileName,
                            "tests/outputTests/expectedOutput/expectedintenseInterval_.txt"));
}

TEST_F(VaccinSimulationTests, happyDays) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDays.xml"));

    std::string testName = "happyDays";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/happyDays.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());

    EXPECT_EQ(4, s.getFcentra().size());
    EXPECT_TRUE(NULL != s.getFhub());
    EXPECT_EQ(93000, s.getFhub()->getVaccin());
    EXPECT_EQ("Park Spoor Oost", s.getFcentra().find("Park Spoor Oost")->first);
    EXPECT_EQ("AED Studios", s.getFcentra().find("AED Studios")->first);
    EXPECT_EQ("De Zoerla", s.getFcentra().find("De Zoerla")->first);
    EXPECT_EQ("Flanders Expo", s.getFcentra().find("Flanders Expo")->first);

    s.automaticSimulation(18, ostream);
    std::string exportFileName = "tests/outputTests/generatedOutput/generatedhappyDays_.txt";
    s.exportFile(exportFileName);

    ostream.close();
    EXPECT_TRUE(FileExists(exportFileName));
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
    EXPECT_TRUE(FileCompare(exportFileName,
                            "tests/outputTests/expectedOutput/expectedhappyDays_.txt"));
}

TEST_F(VaccinSimulationTests, alreadyVaccinated) {

    ASSERT_TRUE(FileExists("tests/inputTests/alreadyVaccinated.xml"));

    std::ofstream ostream;
    s.importXmlFile("tests/inputTests/alreadyVaccinated.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());
    s.simulateTransport(ostream);
    s.simulateVaccination(ostream);
    while (s.getFhub()->getVaccin() != 0) {
        s.simulateVaccination(ostream);
        s.simulateTransport(ostream);
    }
    s.getFhub()->updateVaccins();
    EXPECT_DEATH(s.automaticSimulation(6, ostream),
                 "Amount of vaccins or amount of vaccinated in a center must be 0 at begin of simulation");
}


