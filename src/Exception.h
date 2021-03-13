/**
 * @file Exception.h
 * @brief This header file contains the declarations and the members of the Exception class
 * @author Stein Vandenbroeke
 * @date 04/03/2021
 */

#ifndef TTT_EXCEPTION_H
#define TTT_EXCEPTION_H

#include <string>
#include "DesignByContract.h"

/*
 * \brief Class implemented to handle exceptions
 */
class Exception {

private:
    std::string errorValue; ///< Value of exception as string
    __unused int errorCode; ///< Code of exception as int
    Exception *_initCheck;

public:
    /**
     * \brief Constructor for a Exception object
     *
     * @param error Message of error
     * @param errorCode Code that is given with error, standard 0
     *
     * @post
     * ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state")
     */
    explicit Exception(const std::string &error, int errorCode = 0);

    /**
     * \brief Check whether the Exception object is properly initialised
     *
     * @return true when object is properly initialised, false when not
     */
    bool properlyInitialized() const;

    /**
     * \brief Returns the error message as string
     *
     * @return ErrorValue as const char*
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Exception object must be properly initialized")
     */
    std::string value() const  throw() ;
};

#endif //TTT_EXCEPTION_H
