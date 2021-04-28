/**
 * @file VdSimulateTests.cpp
 * @brief This file is used to all the tests of the Simulate class - input, output - based off Serge Demeyer tests
 * @author Pablo Deputter
 * @date 11/03/2021
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
TEST_F(VaccinSimulationTests, EmptySimulation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyFile.xml"));
    std::ofstream ostream;

    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_DEATH(s.importXmlFile("tests/inputTests/emptyFile.xml"),
                 "The file that needs to be read must not be empty");
    EXPECT_DEATH(s.importXmlFile("nonexistent.xml"), "The file that needs to be read must exist");
    EXPECT_DEATH(s.checkConnections(), "Hub must exist");
    EXPECT_FALSE(s.checkCentra());
    EXPECT_FALSE(s.checkHub());
    EXPECT_FALSE(s.checkSimulation());
    EXPECT_DEATH(s.exportFile("tests/inputTests/file1.txt"),
                 "The simulation must be valid/consistent");
    EXPECT_TRUE(s.getFcentra().empty());
    EXPECT_DEATH(s.automaticSimulation(5, ostream, false, false),
                 "The simulation must be valid/consistent");
    EXPECT_DEATH(s.simulateVaccination(ostream), "The simulation must be valid/consistent");
    EXPECT_DEATH(s.simulateTransport(5, ostream), "The simulation must be valid/consistent");
}


// Test automatic simulation with no equal vaccins as delivery on day zero
TEST_F(VaccinSimulationTests, WrongStartSimulation) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDays2.xml"));

    std::string testName = "WrongStartSimulation";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/happyDays2.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());
    s.automaticSimulation(20, ostream, false, false);
    ostream << std::endl;
    s.simulateVaccination(ostream);
    EXPECT_TRUE(s.checkSimulation());

    EXPECT_DEATH(s.automaticSimulation(5, ostream, false, false),
                 "Amount of vaccins or amount of vaccinated in a center must be 0 at begin of simulation");

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

// Tests .xml file with closing tags errors
TEST_F(VaccinSimulationTests, WrongClosingXml) {

    ASSERT_TRUE(FileExists("tests/inputTests/wrongClosingTagXml2.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/wrongLayoutXml.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/containsWrongTagsXml.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));
    EXPECT_THROW(s.importXmlFile("tests/inputTests/wrongClosingTagXml2.xml"),
                 Exception);
    EXPECT_FALSE(s.checkSimulation());

    EXPECT_THROW(s.importXmlFile("tests/inputTests/wrongLayoutXml.xml", "tests/inputTests/knownTags.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());

    std::ostringstream errorStream;
    EXPECT_NO_THROW(s.importXmlFile("tests/inputTests/containsWrongTagsXml.xml", "tests/inputTests/knownTags.xml", errorStream));
    EXPECT_EQ("Error unknown tag: 'HAHAHHAFOUT'\n", errorStream.str());
    EXPECT_TRUE(s.checkSimulation());
}

// Tests .xml file with wrong Center information
TEST_F(VaccinSimulationTests, WrongCentraInformation) {
    ASSERT_TRUE(FileExists("tests/inputTests/wrongCentra.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));

    std::ostringstream errorStream;
    s.importXmlFile("tests/inputTests/wrongCentra.xml", "tests/inputTests/knownTags.xml", errorStream);
    EXPECT_EQ("Element not found: inwoners\nCentra Flanders Expo does not exist\n", errorStream.str());
    EXPECT_TRUE(s.checkSimulation());
}

//// Test .xml file with wrong Hub information
TEST_F(VaccinSimulationTests, wrongHubInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/wrongHub.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));

    std::ostringstream errorStream;

    s.importXmlFile("tests/inputTests/wrongHub.xml", "tests/inputTests/knownTags.xml", errorStream);
    EXPECT_EQ("Centra T does not exist\n"
    , errorStream.str());
    EXPECT_TRUE(s.checkSimulation());
}

// Tests .xml file with lacking Hub information
TEST_F(VaccinSimulationTests, BrokenHubInformation) {
    ASSERT_TRUE(FileExists("tests/inputTests/brokenHub.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));
    std::ostringstream errorStream;
    EXPECT_THROW(s.importXmlFile("tests/inputTests/brokenHub.xml", "tests/inputTests/knownTags.xml", errorStream), Exception);
    EXPECT_EQ("Hub not added: No centra\n", errorStream.str());
    EXPECT_FALSE(s.checkSimulation());
}

// Tests .xml file with no Hub
TEST_F(VaccinSimulationTests, NoHub) {
    ASSERT_TRUE(FileExists("tests/inputTests/noHub.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));
    EXPECT_THROW(s.importXmlFile("tests/inputTests/noHub.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());
}

// Tests .xml with no VaccinationCentra
TEST_F(VaccinSimulationTests, NoCentra) {
    ASSERT_TRUE(FileExists("tests/inputTests/noCentra.xml"));

    EXPECT_THROW(s.importXmlFile("tests/inputTests/noCentra.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());
}

// Tests simulation with .txt file instead of .xml file
TEST_F(VaccinSimulationTests, WrongFile) {

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/noCentra.txt"),
                 "The file that needs to be read must exist");
    EXPECT_FALSE(s.checkSimulation());
}

// Tests simulation with empty .xml file
TEST_F(VaccinSimulationTests, EmptyFile) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyFile.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/emptyFile.xml"),
                 "The file that needs to be read must not be empty");
    EXPECT_FALSE(s.checkSimulation());
}


// Tests scenario with a short interval between deliveries to Hub
TEST_F(VaccinSimulationTests, IntenseInterval) {

    ASSERT_TRUE(FileExists("tests/inputTests/intenseInterval.xml"));

    std::string testName = "IntenseInterval";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/intenseInterval.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());

    s.automaticSimulation(80, ostream, false, false);
    std::string exportFileName = "tests/outputTests/generatedOutput/generatedIntenseInterval_.txt";
    std::string exportIniName = "tests/outputTests/generatedOutput/generatedIntenseInterval.ini";
    s.exportFile(exportFileName);
    s.generateIni(exportIniName);

    ostream.close();
    EXPECT_TRUE(FileExists(exportFileName));
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
    EXPECT_TRUE(FileCompare(exportFileName,
                            "tests/outputTests/expectedOutput/expectedIntenseInterval_.txt"));
    EXPECT_TRUE(FileExists(exportIniName));
    EXPECT_FALSE(FileIsEmpty(exportIniName));
    EXPECT_TRUE(FileCompare(exportIniName,
                            "tests/outputTests/expectedOutput/expectedIntenseInterval.ini"));
}

// Happy day scenario for Simulation
TEST_F(VaccinSimulationTests, HappyDays) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDays2.xml"));

    std::string testName = "HappyDays";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/happyDays2.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());

    unsigned long int x = 4;
    EXPECT_EQ(x, s.getFcentra().size());
    EXPECT_TRUE(!s.getHub().empty());
    EXPECT_EQ("Park Spoor Oost", s.getFcentra().find("Park Spoor Oost")->first);
    EXPECT_EQ("AED Studios", s.getFcentra().find("AED Studios")->first);
    EXPECT_EQ("De Zoerla", s.getFcentra().find("De Zoerla")->first);
    EXPECT_EQ("Flanders Expo", s.getFcentra().find("Flanders Expo")->first);

    s.automaticSimulation(77, ostream, false, false);
    std::string exportFileName = "tests/outputTests/generatedOutput/generatedHappyDays_.txt";
    std::string exportIniName = "tests/outputTests/generatedOutput/generatedHappyDays.ini";
    s.exportFile(exportFileName);
    s.generateIni(exportIniName);

    ostream.close();
    EXPECT_TRUE(FileExists(exportFileName));
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
    EXPECT_TRUE(FileCompare(exportFileName,
                            "tests/outputTests/expectedOutput/expectedHappyDays_.txt"));
    EXPECT_TRUE(FileExists(exportIniName));
    EXPECT_TRUE(FileExists("tests/outputTests/expectedOutput/expectedHappyDays.ini"));
    EXPECT_TRUE(FileCompare(exportIniName,
                            "tests/outputTests/expectedOutput/expectedHappyDays.ini"));
}

// Happy day scenario for Simulation with multiple hubs
TEST_F(VaccinSimulationTests, HappyDaysMultipleHubs) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDaysMultipleHubs.xml"));

    std::string testName = "HappyDaysMultipleHubs";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/happyDaysMultipleHubs.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());

    unsigned long int x = 4;
    EXPECT_EQ(x, s.getFcentra().size());
    EXPECT_TRUE(!s.getHub().empty());
    EXPECT_EQ("Park Spoor Oost", s.getFcentra().find("Park Spoor Oost")->first);
    EXPECT_EQ("AED Studios", s.getFcentra().find("AED Studios")->first);
    EXPECT_EQ("De Zoerla", s.getFcentra().find("De Zoerla")->first);
    EXPECT_EQ("Flanders Expo", s.getFcentra().find("Flanders Expo")->first);

    s.automaticSimulation(150, ostream, false, false);
    std::string exportFileName = "tests/outputTests/generatedOutput/generatedHappyDaysMultipleHubs_.txt";
    std::string exportIniName = "tests/outputTests/generatedOutput/generatedHappyDaysMultipleHubs.ini";
    s.exportFile(exportFileName);
    s.generateIni(exportIniName);

    ostream.close();
    EXPECT_TRUE(FileExists(exportFileName));
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare) || FileCompare(fileName,
                "tests/outputTests/expectedOutput/expectedHappyDaysMultipleHubs2.txt"));
    EXPECT_TRUE(FileCompare(exportFileName,
                            "tests/outputTests/expectedOutput/expectedHappyDaysMultipleHubs_.txt"));
    EXPECT_TRUE(FileExists(exportIniName));
    EXPECT_TRUE(FileExists("tests/outputTests/expectedOutput/expectedHappyDaysMultipleHubs.ini"));
    EXPECT_TRUE(FileCompare(exportIniName,
                            "tests/outputTests/expectedOutput/expectedHappyDaysMultipleHubs.ini"));
}

// Happy day scenario for Simulation with even more hubs
TEST_F(VaccinSimulationTests, HappyDaysOverkillHubs) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDaysOverKillHubs.xml"));

    std::string testName = "HappyDaysOverKillHubs";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/happyDaysOverKillHubs.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());

    unsigned long int x = 4;
    EXPECT_EQ(x, s.getFcentra().size());
    EXPECT_TRUE(!s.getHub().empty());
    EXPECT_EQ("Park Spoor Oost", s.getFcentra().find("Park Spoor Oost")->first);
    EXPECT_EQ("AED Studios", s.getFcentra().find("AED Studios")->first);
    EXPECT_EQ("De Zoerla", s.getFcentra().find("De Zoerla")->first);
    EXPECT_EQ("Flanders Expo", s.getFcentra().find("Flanders Expo")->first);

    s.automaticSimulation(130, ostream, false, false);
    std::string exportFileName = "tests/outputTests/generatedOutput/generatedHappyDaysOverKillHubs_.txt";
    std::string exportIniName = "tests/outputTests/generatedOutput/generatedHappyDaysOverKillHubs.ini";
    s.exportFile(exportFileName);
    s.generateIni(exportIniName);

    ostream.close();
    EXPECT_TRUE(FileExists(exportFileName));
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
    EXPECT_TRUE(FileCompare(exportFileName,
                            "tests/outputTests/expectedOutput/expectedHappyDaysOverKillHubs_.txt"));
    EXPECT_TRUE(FileExists(exportIniName));
    EXPECT_TRUE(FileExists("tests/outputTests/expectedOutput/expectedHappyDaysOverKillHubs.ini"));
    EXPECT_TRUE(FileCompare(exportIniName,
                            "tests/outputTests/expectedOutput/expectedHappyDaysOverKillHubs.ini"));
}

// Test simulation where center has already vaccinated/has not 0 vaccins in stock
TEST_F(VaccinSimulationTests, AlreadyVaccinated) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDays2.xml"));

    std::ofstream ostream;
    s.importXmlFile("tests/inputTests/happyDays2.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());
    s.simulateTransport(0, ostream);
    s.simulateVaccination(ostream);
    s.simulateVaccination(ostream);
    s.simulateTransport(0, ostream);
    s.getHub()[0]->updateVaccins();
    EXPECT_DEATH(s.automaticSimulation(6, ostream, false, false),
                 "Hub must have equal amount of vaccins as delivery on day zero");
}

// Test simulation with low transport
TEST_F(VaccinSimulationTests, LowTransport) {

    ASSERT_TRUE(FileExists("tests/inputTests/lowTransport.xml"));

    std::string testName = "LowTransport";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/lowTransport.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());

    s.automaticSimulation(33, ostream, false, false);
    std::string exportFileName = "tests/outputTests/generatedOutput/generatedLowTransport_.txt";
    std::string exportIniName = "tests/outputTests/generatedOutput/generatedLowTransport.ini";
    s.exportFile(exportFileName);
    s.exportFile(exportIniName);

    ostream.close();
    EXPECT_TRUE(FileExists(exportFileName));
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
    EXPECT_TRUE(FileCompare(exportFileName,
                            "tests/outputTests/expectedOutput/expectedLowTransport_.txt"));
    EXPECT_TRUE(FileExists(exportIniName));
    EXPECT_FALSE(FileIsEmpty(exportIniName));
    EXPECT_TRUE(FileCompare(exportIniName,
                            "tests/outputTests/expectedOutput/expectedLowTransport.ini"));
}

// Test simulation with non-existing center
TEST_F(VaccinSimulationTests, NonExistingCenter) {

    ASSERT_TRUE(FileExists("tests/inputTests/nonExistingCenter.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/nonExistingCenter.xml"), "The simulation must be valid/consistent");
    //EXPECT_EQ("Centra Park spoor Oost does not exist\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with not connected center
TEST_F(VaccinSimulationTests, NotConnectedCenter) {

    ASSERT_TRUE(FileExists("tests/inputTests/notConnectedCenter.xml"));
    EXPECT_DEATH(s.importXmlFile("tests/inputTests/notConnectedCenter.xml"),
                                 "The simulation must be valid/consistent");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with string as vaccin information
TEST_F(VaccinSimulationTests, StringVaccinInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/stringVaccinInformation.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/stringVaccinInformation.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Vaccin not added: Can't convert string to int\n",err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with string as center information
TEST_F(VaccinSimulationTests, StringCenterInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/stringCenterInformation.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/stringCenterInformation.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Can't convert string to int\nCentra Flanders Expo does not exist\n", err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with empty capacity information
TEST_F(VaccinSimulationTests, EmptyCapacityInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyCapacityInformation.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/emptyCapacityInformation.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ(
            "Element is empty: capaciteit\nCentra Flanders Expo does not exist\nCentra Flanders Expo does not exist\n",
            err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with empty vaccin information
TEST_F(VaccinSimulationTests, EmptyVaccinInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyVaccinInformation.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/emptyVaccinInformation.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Vaccin not added: Element is empty: levering\n",err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with no end tag for VaccinationCenter
TEST_F(VaccinSimulationTests, VaccinationCenterEndTag) {

    ASSERT_TRUE(FileExists("tests/inputTests/vaccinationCenterEndTag.xml"));
    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/vaccinationCenterEndTag.xml"), Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("", err);
    EXPECT_TRUE(!s.checkSimulation());
}

TEST_F(VaccinSimulationTests, MultipleEndTags) {

    ASSERT_TRUE(FileExists("tests/inputTests/multipleEndTags.xml"));
    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/multipleEndTags.xml"), Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("", err);
    EXPECT_TRUE(!s.checkSimulation());
}

// Test simulation with no vaccin in Hub
TEST_F(VaccinSimulationTests, NoVaccinHub) {

    ASSERT_TRUE(FileExists("tests/inputTests/noVaccinHub.xml"));
    testing::internal::CaptureStderr();
    EXPECT_NO_THROW(s.importXmlFile("tests/inputTests/noVaccinHub.xml"));
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Vaccin not added: Element not found: type\n", err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with no vaccin tag in hub
TEST_F(VaccinSimulationTests, NoVaccinTag) {

    ASSERT_TRUE(FileExists("tests/inputTests/noVaccinTag.xml"));
    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/noVaccinTag.xml"),
                 Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Hub not added: Element not found: levering\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with missing element in center
TEST_F(VaccinSimulationTests, MissingElementCenter) {

    ASSERT_TRUE(FileExists("tests/inputTests/missingElementCenter.xml"));
    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/missingElementCenter.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element not found: inwoners\nElement not found: inwoners\nCentra Park Spoor Oost does not exist\nCentra De Zoerla does not exist\n",
              err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with multiple missing element in center
TEST_F(VaccinSimulationTests, MissingMultipleElementsCenter) {

    std::ostringstream errorStream;
    ASSERT_TRUE(FileExists("tests/inputTests/missingMultipleElementsCenter.xml"));
    s.importXmlFile("tests/inputTests/missingMultipleElementsCenter.xml",
                    "tests/inputTests/knownTags.xml", errorStream);

    EXPECT_EQ("Element not found: adres\nElement not found: inwoners\nCentra De Zoerla does not exist\nCentra Flanders Expo does not exist\nCentra Flanders Expo does not exist\n",
              errorStream.str());
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with missing element in vaccin
TEST_F(VaccinSimulationTests, MissingElementVaccin) {

    ASSERT_TRUE(FileExists("tests/inputTests/missingElementVaccin.xml"));
    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/missingElementVaccin.xml");
    std::string err = testing::internal::GetCapturedStderr();

    EXPECT_EQ("Vaccin not added: Element not found: interval\n", err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with multiple missing elements in vaccin
TEST_F(VaccinSimulationTests, MissingMultipleElementsVaccin) {

    ASSERT_TRUE(FileExists("tests/inputTests/missingMultipleElementsVaccin.xml"));
    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/missingMultipleElementsVaccin.xml");
    std::string err = testing::internal::GetCapturedStderr();

    EXPECT_EQ("Vaccin not added: Element not found: transport\nVaccin not added: Element not found: levering\nVaccin not added: Element not found: type\n",
              err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with .xml file with a lot of exceptions
TEST_F(VaccinSimulationTests, SadDays) {

    ASSERT_TRUE(FileExists("tests/inputTests/sadDays.xml"));
    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/sadDays.xml");
    std::string err = testing::internal::GetCapturedStderr();

    std::string errCompare = "Element not found: adres\nVaccin not added: Element not found: type\nVaccin not added: ";
    errCompare.append("Element is empty: type\nEmpty centra name\nCentra Sint-Jozef Malle does not exist\n");
    errCompare.append("Centra De Zoerla does not exist\nVaccin not added: Can't convert string to int\n");
    EXPECT_EQ(errCompare, err);
    EXPECT_TRUE(s.checkSimulation());
}