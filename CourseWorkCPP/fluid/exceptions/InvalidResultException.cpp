#include "InvalidResultException.h"

InvalidResultException::InvalidResultException() : std::runtime_error("Result contains NaN or Infinity")
{

}
