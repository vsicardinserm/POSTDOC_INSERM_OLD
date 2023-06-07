#ifndef ATTRIBUTE_EXCEPTION_H
#define ATTRIBUTE_EXCEPTION_H

#include <exception>

class AttributeException : public std::exception
{
public:
    const char* what() const noexcept override;
};

#endif // ATTRIBUTE_EXCEPTION_H
