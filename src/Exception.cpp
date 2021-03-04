//
// Created by stein on 4/03/2021.
//

#include "Exception.h"


Exception::Exception(std::string error, int errorCode) {
    errorValue = error;
    this->errorCode = errorCode;
}

std::string Exception::value() const throw() {
    return errorValue;
}
