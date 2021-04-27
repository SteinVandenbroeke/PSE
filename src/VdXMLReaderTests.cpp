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

// Test with an "empty" XML file
TEST_F(XMLReaderTest, EmptyXml) {

    ASSERT_TRUE(FileExists("tests/inputTests/emptySimulation.xml"));
    EXPECT_THROW(XMLReader r("tests/inputTests/emptySimulation.xml"), Exception);
}

// Test acceptedTags not all tags allowed for spec 1.0
TEST_F(XMLReaderTest, EmptyAllowedTags1) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDays1.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTagsNotComplete1.xml"));

    std::ostringstream errorStream;
    XMLReader r("tests/inputTests/happyDays1.xml");
    r.acceptedTags(errorStream, "tests/inputTests/knownTagsNotComplete1.xml");
    EXPECT_EQ("Error unknown tag: 'naam'\nError unknown tag: 'naam'\nError unknown tag: 'naam'\nError unknown tag: 'naam'\n"
              ,errorStream.str());
}

// Test acceptedTags not all tags allowed for spec 2.0
TEST_F(XMLReaderTest, EmptyAllowedTags2) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDays2.xml"));
    ASSERT_TRUE(FileExists("tests/inputTests/knownTagsNotComplete2.xml"));

    std::ostringstream errorStream;
    XMLReader r("tests/inputTests/happyDays2.xml");
    r.acceptedTags(errorStream, "tests/inputTests/knownTagsNotComplete2.xml");
    EXPECT_EQ("Error unknown tag: 'hernieuwing'\nError unknown tag: 'hernieuwing'\nError unknown tag: 'hernieuwing'\n"
    ,errorStream.str());
}

// Test acceptedTags with file missing
TEST_F(XMLReaderTest, NoAllowedTagTest) {

    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    ASSERT_TRUE(!FileExists("tests/inputTests/noFileOnThisPath"));

    std::ostringstream errorStream;
    XMLReader r("tests/inputTests/basicXml.xml");
    r.acceptedTags(errorStream, "tests/inputTests/noFileOnThisPath.xml");
    EXPECT_EQ("cannot find file with accepted tags\nerror in find file with accepted tags: Failed to open file\nError unknown tag: 'Node1'\nError unknown tag: 'Node2'\nError unknown tag: 'Node3'\n"
    ,errorStream.str());
}

// Test with wrong pad
TEST_F(XMLReaderTest, NoFileOnPath) {

    ASSERT_TRUE(!FileExists("tests/inputTests/noFileOnThisPath"));
    EXPECT_DEATH(XMLReader r("tests/inputTests/noFileOnThisPath"), "File must exist on path");
}

// Test with an "broken" XML file for spec 1
TEST_F(XMLReaderTest, BrokenXml1) {

    ASSERT_TRUE(FileExists("tests/inputTests/wrongClosingTagXml1.xml"));
    std::ofstream ostream;
    EXPECT_THROW(XMLReader r("tests/inputTests/wrongClosingTagXml1.xml"), Exception);
}

// Test with an "broken" XML file for spec 2
TEST_F(XMLReaderTest, BrokenXml2) {

    ASSERT_TRUE(FileExists("tests/inputTests/wrongClosingTagXml2.xml"));
    std::ofstream ostream;
    EXPECT_THROW(XMLReader r("tests/inputTests/wrongClosingTagXml2.xml"), Exception);
}

// getElementValue happyday test
TEST_F(XMLReaderTest, GetElementValueHappyday) {

    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    XMLReader r("tests/inputTests/basicXml.xml");
    TiXmlElement* node = r.getElement("Node1");
    std::string expected = r.getElementValue(*node,"Node2");
    EXPECT_EQ("Test", expected);
}

// getElementValue none existing name
TEST_F(XMLReaderTest, GetElementValueNonExstingName) {

    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    XMLReader r("tests/inputTests/basicXml.xml");
    TiXmlElement* node = r.getElement("Node1");
    EXPECT_THROW(r.getElementValue(*node,"Node5"), Exception);
}

// getElement happyday test
TEST_F(XMLReaderTest, GetElementHappyday) {

    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    XMLReader r("tests/inputTests/basicXml.xml");
    EXPECT_NO_THROW(r.getElement("Node1"));
    EXPECT_NO_THROW(r.getElement("Node3"));
    EXPECT_THROW(r.getElement("Node2"), Exception);
}

// getElement none existing name
TEST_F(XMLReaderTest, GetElementNullNode) {

    ASSERT_TRUE(FileExists("tests/inputTests/basicXml.xml"));
    XMLReader r("tests/inputTests/basicXml.xml");
    EXPECT_THROW(r.getElement("Node5"), Exception);
}


// Test with an "working" XML file for spec 1
TEST_F(XMLReaderTest, Happyday1) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDays1.xml"));
    XMLReader r("tests/inputTests/happyDays1.xml");

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

// Test with an "working" XML file for spec 2
TEST_F(XMLReaderTest, Happyday2) {

    ASSERT_TRUE(FileExists("tests/inputTests/happyDays2.xml"));
    XMLReader r("tests/inputTests/happyDays2.xml");

    EXPECT_NO_THROW(r.getElement("HUB"));
    TiXmlElement* elm = r.getElement("HUB");

    TiXmlElement* xmlVaccin = elm->FirstChildElement("VACCIN");

    EXPECT_NO_THROW(r.getElementValue(*xmlVaccin, "temperatuur"));
    std::string temperature = r.getElementValue(*xmlVaccin, "temperatuur");
    EXPECT_EQ(temperature, "-70");

    EXPECT_NO_THROW(r.getElement("VACCINATIECENTRUM"));
    elm = r.getElement("VACCINATIECENTRUM");

    EXPECT_NO_THROW(r.getElementValue(*elm, "naam"));
    std::string name = r.getElementValue(*elm, "naam");
    EXPECT_EQ(name, "Park Spoor Oost");

    EXPECT_THROW(r.getElementValue(*elm, "NOTEXISTS"), Exception);

    EXPECT_THROW(r.getElement("NOTEXIST"), Exception);
}
