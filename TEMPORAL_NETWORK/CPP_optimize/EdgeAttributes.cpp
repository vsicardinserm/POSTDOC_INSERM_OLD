#include "EdgeAttributes.h"
#include "AttributeException.h"

EdgeAttributes::EdgeAttributes() = default;

void EdgeAttributes::addAttribute(const std::string& name, const std::string& value)
{
    attributes[name] = value;
}

std::string EdgeAttributes::getAttribute(const std::string& name) const
{
    try {
        return attributes.at(name);
    } catch (const std::out_of_range& e) {
        std::cout << "\tERROR:" << e.what() << std::endl;
        throw AttributeException();
    }
}

std::ostream& operator<<(std::ostream& os, const EdgeAttributes& obj)
{
    std::string result;
    for (const auto& pair : obj.attributes) {
        result += pair.first + ": " + pair.second + ", ";
    }
    if (!result.empty()) {
        // Remove the trailing comma and space
        result.erase(result.end() - 2, result.end());
    }
    os << "EdgeAttributes:\n\t" << result;

    return os;
}
