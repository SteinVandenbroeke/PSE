//============================================================================
// Name        : TicTacToeUtils.cpp
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#include <string>
#include <cmath>
#include <sstream>

#include "Utils.h"

/**
Auxiliary functions for file manipulation.
*/
bool DirectoryExists(const std::string dirname) {
    struct stat st;
    return stat(dirname.c_str(), &st) == 0;
}

bool FileExists(const std::string filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return false;
    std::ifstream f(filename.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

bool FileIsEmpty(const std::string filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
    return st.st_size == 0;
}

bool FileCompare(const std::string leftFileName, const std::string rightFileName) {
    std::ifstream leftFile, rightFile;
    char leftRead, rightRead;
    bool result;

    // Open the two files.
    leftFile.open(leftFileName.c_str());
    if (!leftFile.is_open()) {
        return false;
    };
    rightFile.open(rightFileName.c_str());
    if (!rightFile.is_open()) {
        leftFile.close();
        return false;
    };

    result = true; // files exist and are open; assume equality unless a counterexamples shows up.
    while (result && leftFile.good() && rightFile.good()) {
        leftFile.get(leftRead);
        rightFile.get(rightRead);
        result = (leftRead == rightRead);
    };
    if (result) {
        // last read was still equal; are we at the end of both files ?
        result = (!leftFile.good()) && (!rightFile.good());
    };

    leftFile.close();
    rightFile.close();
    return result;
}

std::string ToString( int x ) {
    int length = snprintf( NULL, 0, "%d", x );
    char* buf = new char[length + 1];
    snprintf( buf, length + 1, "%d", x );
    std::string str( buf );
    delete[] buf;
    return str;
}

std::string ToString(double x) {

    std::ostringstream sstream;
    sstream << x;
    return sstream.str();
}

int ToInt(std::string& s) {
    for(unsigned long int i = 0; i < s.length(); i++){
        if(!isdigit(s[i]) && (s[i] != '-' && i == 0 )){
            throw Exception("Can't convert string to int");
        }
    }
    return atoi(s.c_str());
}

int ToPercent(const int x, const int max) {
    double a = static_cast<double>(x) / max;
    return static_cast<int>(round(a * 100));
}

std::string ProgressBar(const int x, const int barWidth) {
    std::string progressBar = "[";

    double progress = static_cast<double>(x) / 100;
    int progressAmount = static_cast<int>(progress * barWidth);

    for (int i = 0; i != barWidth; i++) {
        if (i < progressAmount) {
            progressBar.append("=");
            continue;
        }
        progressBar.append(" ");
    }
    progressBar.append("]");
    return progressBar;
}