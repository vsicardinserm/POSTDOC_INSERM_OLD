#include "VertexAttributes.h"
#include "AttributeException.h"

VertexAttributes::VertexAttributes() = default;

VertexAttributes::VertexAttributes(unsigned id) : ID(id) {}

void VertexAttributes::addAttributes(const std::string& name, const std::string& value)
{
    attributes[name] = value;
}

std::string VertexAttributes::getAttribute(const std::string& name) const
{
    try {
        return attributes.at(name);
    } catch (const std::out_of_range& e) {
        std::cout << "\tERROR:" << e.what() << std::endl;
        throw AttributeException();
    }
}

std::ostream& operator<<(std::ostream& os, const VertexAttributes& obj)
{
    std::string result;
    for (const auto& pair : obj.attributes) {
        result += pair.first + ": " + pair.second + ", ";
    }
    if (!result.empty()) {
        // Remove the trailing comma and space
        result.erase(result.end() - 2, result.end());
    }
    os << "VertexAttributes:\n\t" << result;

    return os;
}
