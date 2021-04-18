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

std::string ToString(double i);

int ToInt(std::string&);

int ToPercent(const int x, const int max);

std::string ProgressBar(const int x, const int barWidth);


// Closing of the ``header guard''.

#endif //TTT_UTILS_H
