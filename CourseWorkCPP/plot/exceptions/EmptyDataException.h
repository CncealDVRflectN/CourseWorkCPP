#ifndef COURSEWORKCPP_EMPTYDATAEXCEPTION_H
#define COURSEWORKCPP_EMPTYDATAEXCEPTION_H

#include <stdexcept>

class EmptyDataException : public std::runtime_error
{
public:
    EmptyDataException();
};


#endif //COURSEWORKCPP_EMPTYDATAEXCEPTION_H
