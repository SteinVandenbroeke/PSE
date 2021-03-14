/**
 * @file Exception.cpp
 * @brief This file contains the definitions of the members of the Exception class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#include "Exception.h"

Exception::Exception(const std::string &error) :
    errorValue(error) {

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool Exception::properlyInitialized() const {

    return Exception::_initCheck == this;
}

std::string Exception::value() const throw() {
    return errorValue;
}
