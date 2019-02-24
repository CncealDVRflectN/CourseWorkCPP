#include "IncorrectResultException.h"

IncorrectResultException::IncorrectResultException() : std::runtime_error("Result contains NaN or Infinity")
{

}
