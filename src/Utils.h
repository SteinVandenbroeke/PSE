//============================================================================
// Name        : TicTacToeUtils.h
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

// Below are two lines serving as a so-called ``header guard''.
// Prevents from loading the same .h file multiple times

#ifndef TTT_UTILS_H
#define TTT_UTILS_H

#include <iostream>
#include "Exception.h"
#include <cstdlib>

bool DirectoryExists(const std::string dirname);

bool FileExists(const std::string dirname);

bool FileIsEmpty(const std::string filename);

bool FileCompare(const std::string leftFileName, const std::string rightFileName);

std::string ToString(int i);

/**
 * \brief Convert double to string
 *
 * @param i double to be converted
 *
 * @return double in string format
 */
std::string ToString(double i);

int ToInt(std::string&);

/**
 * \brief Convert int to percent of max number
 *
 * @param x int to be converted
 * @param max max percent
 *
 * @return Percent as int
 */
int ToPercent(const int x, const int max);

/**
 * \brief Convert percent to progressBar
 *
 * @param x percent as int
 * @param barWidth Max barWidth of equal signs
 *
 * @return
 */
std::string ProgressBar(const int x, const int barWidth);

// Closing of the ``header guard''.

#endif //TTT_UTILS_H
