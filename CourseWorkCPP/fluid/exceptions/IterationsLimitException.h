#ifndef COURSEWORKCPP_ITERATIONSLIMITEXCEPTION_H
#define COURSEWORKCPP_ITERATIONSLIMITEXCEPTION_H

#include <stdexcept>

class IterationsLimitException : public std::runtime_error
{
public:
    IterationsLimitException();
};


#endif //COURSEWORKCPP_ITERATIONSLIMITEXCEPTION_H
