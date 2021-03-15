/**
 * @file VdSimulateTests.cpp
 * @brief This file is used to all the tests of the XMLReder - input, output - based off Serge Demeyer tests
 * @author Stein Vandenbroeke
 * @date 13/03/2021
 */

#include "gtest/gtest.h"
#include <fstream>
#include "Simulation.h"

class XMLReaderTest : public::testing::Test {

protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
        ASSERT_TRUE(DirectoryExists("tests"));
        ASSERT_TRUE(DirectoryExists("tests/inputTests"));
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

};

// Test simulation with an "empty" XML file
TEST_F(XMLReaderTest, emptyXml) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptySimulation.xml"));
    EXPECT_THROW(XMLReader r("tests/inputTests/emptySimulation.xml"), Exception);
}

// Test simulation acceptedTags not all tags allowed
TEST_F(XMLReaderTest, emptyAllowedTagTest) {
    ASSERT_TRUE(FileExists("tests/inputTests/happyDays.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTagsNotComplete.xml"));

    std::ostringstream errorStream;
    XMLReader r("tests/inputTests/happyDays.xml");
    r.acceptedTags(errorStream, "tests/inputTests/knownTagsNotComplete.xml");
    EXPECT_EQ("Error unknown tag: 'naam'\nError unknown tag: 'naam'\nError unknown tag: 'naam'\nError unknown tag: 'naam'\n"
              ,errorStream.str());
}

// Test simulation acceptedTags file missing
TEST_F(XMLReaderTest, noAllowedTagTest) {
    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    ASSERT_TRUE(!FileExists("tests/inputTests/nofileonthisPath"));

    std::ostringstream errorStream;
    XMLReader r("tests/inputTests/basicXml.xml");
    r.acceptedTags(errorStream, "tests/inputTests/nofileonthisPath.xml");
    EXPECT_EQ("cannot find file with accepted tags\nerror in find file with accepted tags: Failed to open file\nError unknown tag: 'Node1'\nError unknown tag: 'Node2'\nError unknown tag: 'Node3'\n"
    ,errorStream.str());
}

// Test simulation with wrong pad
TEST_F(XMLReaderTest, noFileOnPath) {

    ASSERT_TRUE(!FileExists("tests/inputTests/nofileonthisPath"));
    std::ofstream ostream;
    EXPECT_THROW(XMLReader r("tests/inputTests/nofileonthisPath"), Exception);
}

// Test simulation with an "broken" XML file
TEST_F(XMLReaderTest, brokenXml) {

    ASSERT_TRUE(FileExists("tests/inputTests/wrongClosingTagXml.xml"));
    std::ofstream ostream;
    EXPECT_THROW(XMLReader r("tests/inputTests/wrongClosingTagXml.xml"), Exception);
}


// getElementValue happyday test
TEST_F(XMLReaderTest, getElementValueHappyday) {
    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    XMLReader r("tests/inputTests/basicXml.xml");
    TiXmlElement* node = r.getElement("Node1");
    std::string expected = r.getElementValue(*node,"Node2");
    EXPECT_EQ("Test", expected);
}

// getElementValue none existing name
TEST_F(XMLReaderTest, getElementValueNonExstingName) {
    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    XMLReader r("tests/inputTests/basicXml.xml");
    TiXmlElement* node = r.getElement("Node1");
    EXPECT_THROW(r.getElementValue(*node,"Node5"), Exception);
}

// getElement happyday test
TEST_F(XMLReaderTest, getElementHappyday) {
    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    XMLReader r("tests/inputTests/basicXml.xml");
    EXPECT_NO_THROW(r.getElement("Node1"));
    EXPECT_NO_THROW(r.getElement("Node3"));
    EXPECT_THROW(r.getElement("Node2"), Exception);
}

// getElement none existing name
TEST_F(XMLReaderTest, getElementNullNode) {
    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    XMLReader r("tests/inputTests/basicXml.xml");
    EXPECT_THROW(r.getElement("Node5"), Exception);
}

// Test simulation with an "working" XML file
TEST_F(XMLReaderTest, happyday) {

    ASSERT_TRUE(FileExists("tests/inputTests/intenseInterval.xml"));
    XMLReader r("tests/inputTests/intenseInterval.xml");

    EXPECT_NO_THROW(r.getElement("HUB"));
    TiXmlElement* elm = r.getElement("HUB");

    EXPECT_NO_THROW(r.getElementValue(*elm, "levering"));
    std::string delivery = r.getElementValue(*elm, "levering");
    EXPECT_EQ(delivery, "93000");

    EXPECT_NO_THROW(r.getElement("VACCINATIECENTRUM"));
    elm = r.getElement("VACCINATIECENTRUM");

    EXPECT_NO_THROW(r.getElementValue(*elm, "naam"));
    std::string name = r.getElementValue(*elm, "naam");
    EXPECT_EQ(name, "Park Spoor Oost");

    EXPECT_THROW(r.getElementValue(*elm, "NOTEXISTS"), Exception);

    EXPECT_THROW(r.getElement("NOTEXIST"), Exception);
}
