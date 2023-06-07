#ifndef EDGE_ATTRIBUTES_H
#define EDGE_ATTRIBUTES_H

#include <unordered_map>
#include <string>
#include <iostream>

class EdgeAttributes
{
private:
    std::unordered_map<std::string, std::string> attributes;

public:
    EdgeAttributes();
    void addAttribute(const std::string& name, const std::string& value);
    std::string getAttribute(const std::string& name) const;

    friend std::ostream& operator<<(std::ostream& os, const EdgeAttributes& obj);
};

#endif // EDGE_ATTRIBUTES_H
