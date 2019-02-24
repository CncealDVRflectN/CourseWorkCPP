#ifndef COURSEWORKCPP_TARGETBONDEXCEPTION_H
#define COURSEWORKCPP_TARGETBONDEXCEPTION_H

#include <stdexcept>

class TargetBondException : public std::runtime_error
{
public:
    TargetBondException();
};


#endif //COURSEWORKCPP_TARGETBONDEXCEPTION_H
