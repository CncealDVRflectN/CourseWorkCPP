#ifndef COURSEWORKCPP_INCORRECTRESULTEXCEPTION_H
#define COURSEWORKCPP_INCORRECTRESULTEXCEPTION_H

#include <stdexcept>

class IncorrectResultException : public std::runtime_error
{
public:
    IncorrectResultException();
};


#endif //COURSEWORKCPP_INCORRECTRESULTEXCEPTION_H
