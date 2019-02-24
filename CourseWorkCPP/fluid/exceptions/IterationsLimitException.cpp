#include "IterationsLimitException.h"

IterationsLimitException::IterationsLimitException() : std::runtime_error("Iterations limit exceeded")
{

}
