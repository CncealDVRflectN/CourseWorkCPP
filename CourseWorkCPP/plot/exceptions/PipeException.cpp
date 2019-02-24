#include "PipeException.h"

PipeException::PipeException() : std::runtime_error("Could not open pipe")
{

}
