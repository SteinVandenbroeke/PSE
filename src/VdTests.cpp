/**
 * @file VdTests.cpp
 * @brief This file is used to all the tests of the system - based off the tesing of TicTacToe by Serge Demeyer
 * @author Pablo Deputter
 * @date 11/03/2012
 */

#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
