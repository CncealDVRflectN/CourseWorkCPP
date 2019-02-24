#include "EmptyDataException.h"

EmptyDataException::EmptyDataException() : std::runtime_error("Not enough data for plot")
{

}
