//
// Created by stein on 4/03/2021.
//

#ifndef TTT_EXCEPTION_H
#define TTT_EXCEPTION_H
#include <string>

class Exception {
    std::string errorValue;
    int errorCode;
public:
    /*
     *Maak een error aan met een fout text, en eventueel een error code
     * @param error:        een string met de fout text
     * @param errorCode:    een int die om een fout code mee te geven
     */
    Exception(std::string error, int errorCode = 0);

    /*
     * Geeft de fout text terug
     * @return de fout text als const char*
     */
    std::string value() const throw ();
};
#endif //TTT_EXCEPTION_H
