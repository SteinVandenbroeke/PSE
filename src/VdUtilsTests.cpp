/**
 * @file VdUtilsTests.cpp
 * @brief This file is used to run tests for Utils class - overgenomen van Serge Demeyer
 * @author Pablo Deputter
 * @date 11/03/2021
 */

#include <fstream>
#include "gtest/gtest.h"
#include "Utils.h"

class UtilsTests : public::testing::Test {

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

// Tests the FileCompare() method
TEST_F(UtilsTests, FileCompare) {
    ASSERT_TRUE(DirectoryExists("tests/inputTests"));

    std::ofstream myfile;
    myfile.open("tests/inputTests/file1.txt");
    myfile.close();
    myfile.open("tests/inputTests/file2.txt");
    myfile.close();

    EXPECT_TRUE(FileExists("tests/inputTests/file1.txt"));
    EXPECT_TRUE(FileExists("tests/inputTests/file2.txt"));
    EXPECT_TRUE(FileIsEmpty("tests/inputTests/file1.txt"));
    EXPECT_TRUE(FileIsEmpty("tests/inputTests/file2.txt"));

//    doesnt work on mac
//    //compare 2 empty files
//    // *** Beware: the following does not work with older versions of libstdc++
//    // *** It doesn't work with gcc version 4.0.1 (Apple Inc. build 5465)
//    // *** It does work with gcc version 4.2.1
//    EXPECT_TRUE(FileCompare("tests/inputTests/file1.txt", "tests/inputTests/file2.txt"));
//    EXPECT_TRUE(FileCompare("tests/inputTests/file2.txt", "tests/inputTests/file1.txt"));

    //compare an empty and a non-empty files
    myfile.open("tests/inputTests/file1.txt");
    myfile << "xxx" << std::endl << "yyy";
    myfile.close();
    EXPECT_TRUE(FileExists("tests/inputTests/file1.txt"));
    EXPECT_FALSE(FileIsEmpty("tests/inputTests/file1.txt"));
    EXPECT_FALSE(FileCompare("tests/inputTests/file1.txt", "tests/inputTests/file2.txt"));
    EXPECT_FALSE(FileCompare("tests/inputTests/file2.txt", "tests/inputTests/file1.txt"));

    //compare two equal files
    myfile.open("tests/inputTests/file2.txt");
    myfile << "xxx" << std::endl << "yyy";
    myfile.close();
    EXPECT_TRUE(FileExists("tests/inputTests/file2.txt"));
    EXPECT_FALSE(FileIsEmpty("tests/inputTests/file2.txt"));
    EXPECT_TRUE(FileCompare("tests/inputTests/file1.txt", "tests/inputTests/file2.txt"));
    EXPECT_TRUE(FileCompare("tests/inputTests/file2.txt", "tests/inputTests/file1.txt"));

    //compare 2 non-empty files which are off by a character in the middle
    myfile.open("tests/inputTests/file2.txt");
    myfile << "xxx" << std::endl << "xyy";
    myfile.close();
    EXPECT_FALSE(FileCompare("tests/inputTests/file1.txt", "tests/inputTests/file2.txt"));
    EXPECT_FALSE(FileCompare("tests/inputTests/file2.txt", "tests/inputTests/file1.txt"));

    //compare 2 non-empty files where one is one character shorter than the other
    myfile.open("tests/inputTests/file2.txt");
    myfile << "xxx" << std::endl << "yy";
    myfile.close();
    EXPECT_FALSE(FileCompare("tests/inputTests/file1.txt", "tests/inputTests/file2.txt"));
    EXPECT_FALSE(FileCompare("tests/inputTests/file2.txt", "tests/inputTests/file1.txt"));

    //compare existing against non existing file
    EXPECT_FALSE(
            FileCompare("tests/inputTests/file1.txt", "tests/inputTests/nonexisting.txt"));
    EXPECT_FALSE(
            FileCompare("tests/inputTests/nonexisting.txt", "tests/inputTests/file1.txt"));
}

// Test the ToString() method
TEST_F(UtilsTests, ToString) {

    ASSERT_TRUE(DirectoryExists("tests/inputTests"));

    EXPECT_EQ("5", ToString(5));
    EXPECT_EQ("349", ToString(349));
    EXPECT_EQ("-12", ToString(-12));

    EXPECT_EQ("5.32", ToString(5.32));
    EXPECT_EQ("5.2222", ToString(5.2222));
    EXPECT_EQ("-5.2222", ToString(-5.2222));
}

// Test ToPercent() method
TEST_F(UtilsTests, ToPercent) {

    ASSERT_TRUE(DirectoryExists("tests/inputTests"));

    EXPECT_EQ(10, ToPercent(12, 120));
    EXPECT_EQ(1, ToPercent(312, 49679));
}

// Test ProgressBar() method
TEST_F(UtilsTests, ProgressBar) {

    ASSERT_TRUE(DirectoryExists("tests/inputTests"));

    EXPECT_EQ("[==                  ]", ProgressBar(12, 20));
    EXPECT_EQ("[                    ]", ProgressBar(1, 20));
    EXPECT_EQ("[================    ]", ProgressBar(80, 20));
}
