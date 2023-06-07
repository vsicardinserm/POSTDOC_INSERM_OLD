#include "AttributeException.h"

const char* AttributeException::what() const noexcept
{
    return "Attribute vertex error";
}
