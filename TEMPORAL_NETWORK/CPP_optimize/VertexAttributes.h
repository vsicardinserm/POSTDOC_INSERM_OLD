#ifndef VERTEX_ATTRIBUTES_H
#define VERTEX_ATTRIBUTES_H

#include <unordered_map>
#include <string>
#include <iostream>

class VertexAttributes
{
private:
    std::unordered_map<std::string, std::string> attributes;
    int ID;

public:
    VertexAttributes();
    explicit VertexAttributes(unsigned id);

    void addAttributes(const std::string& name, const std::string& value);
    std::string getAttribute(const std::string& name) const;

    friend std::ostream& operator<<(std::ostream& os, const VertexAttributes& obj);
};

#endif // VERTEX_ATTRIBUTES_H
