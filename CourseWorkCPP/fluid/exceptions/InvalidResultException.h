#ifndef COURSEWORKCPP_INVALIDRESULTEXCEPTION_H
#define COURSEWORKCPP_INVALIDRESULTEXCEPTION_H

#include <stdexcept>

class InvalidResultException : public std::runtime_error
{
public:
    InvalidResultException();
};


#endif //COURSEWORKCPP_InvalidResultException_H
