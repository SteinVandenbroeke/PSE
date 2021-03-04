//
// Created by stein on 4/03/2021.
//

#include "Exception.h"


Exception::Exception(const char * error, int errorCode) {
    errorValue = error;
    this->errorCode = errorCode;
}

const char *Exception::value() const throw() {
    return errorValue;
}
