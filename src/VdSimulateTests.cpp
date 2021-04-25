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
    EXPECT_DEATH(s.simulateTransport(5, ostream), "The simulation must be valid/consistent");
}


//TODO
/*
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
    s.automaticSimulation(20, ostream);
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
}*/

// Tests .xml file with closing tags errors
TEST_F(VaccinSimulationTests, wrongClosingXml) {

    ASSERT_TRUE(FileExists("tests/inputTests/wrongClosingTagXml.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/wrongLayoutXml.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/containsWrongTagsXml.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));
    testing::internal::CaptureStderr();

    EXPECT_THROW(s.importXmlFile("tests/inputTests/wrongClosingTagXml.xml", "tests/inputTests/knownTags.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());

    EXPECT_THROW(s.importXmlFile("tests/inputTests/wrongLayoutXml.xml", "tests/inputTests/knownTags.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());

    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element not found: HUB\n", err);

    testing::internal::CaptureStderr();
    EXPECT_NO_THROW(s.importXmlFile("tests/inputTests/containsWrongTagsXml.xml", "tests/inputTests/knownTags.xml"));
    std::string err2 = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Error unknown tag: 'HAHAHHAFOUT'\nError unknown tag: 'GG'\n", err2);
    EXPECT_TRUE(s.checkSimulation());
}
/*
// Tests .xml file with wrong Center information
TEST_F(VaccinSimulationTests, wrongCentraInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/wrongCentra.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));

    std::ostringstream errorStream;
    s.importXmlFile("tests/inputTests/wrongCentra.xml", "tests/inputTests/knownTags.xml", errorStream);
    EXPECT_EQ("Element not found: inwoners\nHub contains an non existing or wrong vaccination center: 'Park Spoor Oost'\n"
    , errorStream.str());
    EXPECT_TRUE(s.checkSimulation());
}

// Test .xml file with wrong Hub information
TEST_F(VaccinSimulationTests, wrongHubInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/wrongHub.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));

    std::ostringstream errorStream;

    s.importXmlFile("tests/inputTests/wrongHub.xml", "tests/inputTests/knownTags.xml", errorStream);
    EXPECT_EQ("Empty centra name!\n"
    , errorStream.str());
    EXPECT_TRUE(s.checkSimulation());
}

// Tests .xml file with lacking Hub information
TEST_F(VaccinSimulationTests, brokenHubInformation) {
    ASSERT_TRUE(FileExists("tests/inputTests/brokenHub.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));
    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/brokenHub.xml", "tests/inputTests/knownTags.xml"),Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element not found: transport\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Tests .xml file with no Hub
TEST_F(VaccinSimulationTests, noHub) {
    ASSERT_TRUE(FileExists("tests/inputTests/noHub.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTags.xml"));
    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/noHub.xml"), Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element not found: HUB\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Tests .xml with no VaccinationCentra
TEST_F(VaccinSimulationTests, noCentra) {

    ASSERT_TRUE(FileExists("tests/inputTests/noCentra.xml"));

    EXPECT_THROW(s.importXmlFile("tests/inputTests/noCentra.xml"), Exception);
    EXPECT_FALSE(s.checkSimulation());
}

// Tests simulation with .txt file instead of .xml file
TEST_F(VaccinSimulationTests, wrongFile) {

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/noCentra.txt"),
                 "The file that needs to be read must exist");
    EXPECT_FALSE(s.checkSimulation());
}

// Tests simulation with empty .xml file
TEST_F(VaccinSimulationTests, emptyFile) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyFile.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/emptyFile.xml"),
                 "The file that needs to be read must not be empty");
    EXPECT_FALSE(s.checkSimulation());
}

// Tests scenario with a short interval between deliveries to Hub
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

    unsigned long int x = 4;
    EXPECT_EQ(x, s.getFcentra().size());
    EXPECT_TRUE(!s.getHub().empty());
    EXPECT_EQ(93000, (*s.getHub().begin())->getVaccins());
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

// Happy day scenario for Simulation
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

    unsigned long int x = 4;
    EXPECT_EQ(x, s.getFcentra().size());
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

// Test simulation where center has already vaccinated/has not 0 vaccins in stock
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

// Test simulation with low transport
TEST_F(VaccinSimulationTests, lowTransport) {

    ASSERT_TRUE(FileExists("tests/inputTests/lowTransport.xml"));

    std::string testName = "lowTransport";
    std::ofstream ostream;
    std::string fileName = "tests/outputTests/generatedOutput/generated" + testName + ".txt";
    std::string fileNameCompare = "tests/outputTests/expectedOutput/expected" + testName + ".txt";
    ostream.open(fileName.c_str());

    s.importXmlFile("tests/inputTests/lowTransport.xml");
    EXPECT_TRUE(s.properlyInitialized());
    EXPECT_TRUE(s.checkSimulation());

    s.automaticSimulation(12, ostream);
    std::string exportFileName = "tests/outputTests/generatedOutput/generatedlowTransport.txt";
    s.exportFile(exportFileName);

    ostream.close();
    EXPECT_TRUE(FileExists(exportFileName));
    EXPECT_TRUE(FileExists(fileName));
    EXPECT_TRUE(FileExists(fileNameCompare));
    EXPECT_FALSE(FileIsEmpty(fileName));
    EXPECT_TRUE(FileCompare(fileName, fileNameCompare));
    EXPECT_TRUE(FileCompare(exportFileName,
                            "tests/outputTests/expectedOutput/expectedlowTransport.txt"));
}




// Test simulation with negative delivery information
TEST_F(VaccinSimulationTests, negativeDeliveryInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/negativeDeliveryInformation.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/negativeDeliveryInformation.xml"),
                 "Delivery can't be negative or 0");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with zero delivery information
TEST_F(VaccinSimulationTests, zeroDeliveryInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/zeroDeliveryInformation.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/zeroDeliveryInformation.xml"),
                 "Delivery can't be negative or 0");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with string as delivery information
TEST_F(VaccinSimulationTests, stringDeliveryInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/stringDeliveryInformation.xml"));

    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/stringDeliveryInformation.xml"), Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Can't convert string to int\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with empty delivery information
TEST_F(VaccinSimulationTests, emptyDeliveryInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyDeliveryInformation.xml"));

    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/emptyDeliveryInformation.xml"),
                 Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element is empty: levering\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with negative interval information
TEST_F(VaccinSimulationTests, negativeIntervalInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/negativeIntervalInformation.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/negativeIntervalInformation.xml"),
                 "Interval can't be negative or 0");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with 0 as interval information
TEST_F(VaccinSimulationTests, zeroIntervalInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/zeroIntervalInformation.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/zeroIntervalInformation.xml"),
                 "Interval can't be negative or 0");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with string as interval information
TEST_F(VaccinSimulationTests, stringIntervalInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/stringIntervalInformation.xml"));

    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/stringIntervalInformation.xml"),
                 Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Can't convert string to int\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with empty interval information
TEST_F(VaccinSimulationTests, emptyIntervalInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyIntervalInformation.xml"));

    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/emptyIntervalInformation.xml"),
                 Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element is empty: interval\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with negative transport information
TEST_F(VaccinSimulationTests, negativeTransportInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/negativeTransportInformation.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/negativeTransportInformation.xml"),
                 "Transport can't be negative or 0");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with 0 as transport information
TEST_F(VaccinSimulationTests, zeroTransportInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/zeroTransportInformation.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/zeroTransportInformation.xml"),
                 "Transport can't be negative or 0");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with string as transport information
TEST_F(VaccinSimulationTests, stringTransportInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/stringTransportInformation.xml"));

    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/stringTransportInformation.xml"),
                 Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Can't convert string to int\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with empty transport information
TEST_F(VaccinSimulationTests, emptyTransportInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyTransportInformation.xml"));

    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/emptyTransportInformation.xml"),
                 Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element is empty: transport\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with non-existing center
TEST_F(VaccinSimulationTests, nonExistingCenter) {

    ASSERT_TRUE(FileExists("tests/inputTests/nonExistingCenter.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/nonExistingCenter.xml"),
                 "Hub contains an non existing or wrong vaccination center: 'Park spoor Oost'");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with non-string centerName
TEST_F(VaccinSimulationTests, nonStringCenter) {

    ASSERT_TRUE(FileExists("tests/inputTests/nonStringCenter.xml"));


    EXPECT_NO_THROW(s.importXmlFile("tests/inputTests/nonStringCenter.xml"));
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with empty center information
TEST_F(VaccinSimulationTests, emptyCenterInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyCenterInformation.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/emptyCenterInformation.xml"),
                 "Empty centra name!");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with not connected center
TEST_F(VaccinSimulationTests, notConnectedCenter) {

    ASSERT_TRUE(FileExists("tests/inputTests/notConnectedCenter.xml"));

    EXPECT_DEATH(s.importXmlFile("tests/inputTests/notConnectedCenter.xml"),
                 "The simulation must be valid/consistent");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with empty delivery tag, zelfde voor al de rest
TEST_F(VaccinSimulationTests, emptyDeliveryTag) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyDeliveryTag.xml"));
    std::ofstream ostream;
    EXPECT_THROW(s.importXmlFile("tests/inputTests/emptyDeliveryTag.xml", "tests/inputTests/knownTags.xml", ostream), Exception);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with an integer as name of a center
TEST_F(VaccinSimulationTests, intNameCenter) {

    ASSERT_TRUE(FileExists("tests/inputTests/intNameCenter.xml"));

    s.importXmlFile("tests/inputTests/intNameCenter.xml");
    EXPECT_NO_THROW(s.importXmlFile("tests/inputTests/intNameCenter.xml"));
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with empty name as center
TEST_F(VaccinSimulationTests, emptyNameCenter1) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyNameCenter1.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/emptyNameCenter1.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element is empty: naam\nHub contains an non existing or wrong vaccination center: 'Park Spoor Oost'\n",
              err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with empty name as center
TEST_F(VaccinSimulationTests, emptyNameCenter2) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyNameCenter2.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/emptyNameCenter2.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element is empty: naam\nEmpty centra name!\n", err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with empty address for center
TEST_F(VaccinSimulationTests, emptyAddress) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyAddress.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/emptyAddress.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element is empty: adres\nHub contains an non existing or wrong vaccination center: 'Park Spoor Oost'\n",
              err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with negative population information
TEST_F(VaccinSimulationTests, negativePopulationInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/negativePopulationInformation.xml"));

    EXPECT_DEATH(    s.importXmlFile("tests/inputTests/negativePopulationInformation.xml"),
                     "Negative population");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with zero population information
TEST_F(VaccinSimulationTests, zeroPopulationInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/zeroPopulationInformation.xml"));

    EXPECT_NO_FATAL_FAILURE(s.importXmlFile("tests/inputTests/zeroPopulationInformation.xml"));
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with string as population information
TEST_F(VaccinSimulationTests, stringPopulationInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/stringPopulationInformation.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/stringPopulationInformation.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Can't convert string to int\nHub contains an non existing or wrong vaccination center: 'Park Spoor Oost'\n",
              err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with empty population information
TEST_F(VaccinSimulationTests, emptyPopulationInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyPopulationInformation.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/emptyPopulationInformation.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element is empty: inwoners\nHub contains an non existing or wrong vaccination center: 'Park Spoor Oost'\n",
              err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with negative capacity information
TEST_F(VaccinSimulationTests, negativeCapacityInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/negativeCapacityInformation.xml"));

    EXPECT_DEATH(    s.importXmlFile("tests/inputTests/negativeCapacityInformation.xml"),
                     "Negative capacity");
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with zero capacity information
TEST_F(VaccinSimulationTests, zeroCapacityInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/zeroCapacityInformation.xml"));

    EXPECT_NO_FATAL_FAILURE(s.importXmlFile("tests/inputTests/zeroCapacityInformation.xml"));
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with string as capacity information
TEST_F(VaccinSimulationTests, stringCapacityInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/stringCapacityInformation.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/stringCapacityInformation.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Can't convert string to int\nHub contains an non existing or wrong vaccination center: 'Park Spoor Oost'\n",
              err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with empty capacity information
TEST_F(VaccinSimulationTests, emptyCapacityInformation) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptyCapacityInformation.xml"));

    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/emptyCapacityInformation.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ(
            "Element is empty: capaciteit\nHub contains an non existing or wrong vaccination center: 'Park Spoor Oost'\n",
            err);
    EXPECT_TRUE(s.checkSimulation());
}

// Test simulation with no end tag for VaccinationCenter
TEST_F(VaccinSimulationTests, vaccinationCenterEndTag) {

    ASSERT_TRUE(FileExists("tests/inputTests/vaccinationCenterEndTag.xml"));

    EXPECT_THROW(s.importXmlFile("tests/inputTests/vaccinationCenterEndTag.xml"), Exception);
}

// Test simulation with missing element in Hub
TEST_F(VaccinSimulationTests, missingElementHub) {

    ASSERT_TRUE(FileExists("tests/inputTests/missingElementHub.xml"));
    testing::internal::CaptureStderr();
    EXPECT_THROW(s.importXmlFile("tests/inputTests/missingElementHub.xml"), Exception);
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element not found: interval\n", err);
    EXPECT_FALSE(s.checkSimulation());
}

// Test simulation with missing element in center
TEST_F(VaccinSimulationTests, missingElementCenter) {

    ASSERT_TRUE(FileExists("tests/inputTests/missingElementCenter.xml"));
    testing::internal::CaptureStderr();
    s.importXmlFile("tests/inputTests/missingElementCenter.xml");
    std::string err = testing::internal::GetCapturedStderr();
    EXPECT_EQ("Element not found: inwoners\nHub contains an non existing or wrong vaccination center: 'Park Spoor Oost'\n",
              err);
    EXPECT_TRUE(s.checkSimulation());
}*/