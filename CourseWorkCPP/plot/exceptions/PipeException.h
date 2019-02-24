#ifndef COURSEWORKCPP_PIPEEXCEPTION_H
#define COURSEWORKCPP_PIPEEXCEPTION_H

#include <stdexcept>

class PipeException : public std::runtime_error
{
public:
    PipeException();
};


#endif //COURSEWORKCPP_PIPEEXCEPTION_H
