#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/graph/graphviz.hpp>
#include <unordered_map>
#include <map>
#include <random>
#include "progressbar.hpp"
#include <chrono>


class AttributeException : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Attribute vertex error";
    }
};

std::string convertToString(const std::string& attribute) {
    return attribute;
}
std::string convertToString(int attribute) {
    return std::to_string(attribute);
}

struct VertexAttributes {
    std::unordered_map<std::string, std::string> attributes;
    int ID;

    VertexAttributes() = default;
    explicit VertexAttributes(unsigned id) : ID(id) {}

    void addAttributes(const std::string& name, const std::string& value) {
        attributes[name] = value;
    }

    std::string getAttribute(const std::string& name) const {
        try {
            return attributes.at(name);
        } catch (const std::out_of_range& e) {
            std::cout << "ERROR:" << e.what() << std::endl;
            throw AttributeException();
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const VertexAttributes& obj) {
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
};

struct EdgeAttributes {
    std::unordered_map<std::string, std::string> attributes;

    EdgeAttributes() = default;

    void addAttribute(const std::string& name, const std::string& value) {
        attributes[name] = value;
    }

    std::string getAttribute(const std::string& name) {
        try {
            return attributes.at(name);
        } catch (const std::out_of_range& e) {
            std::cout << "ERROR:" << e.what() << std::endl;
            throw AttributeException();
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const EdgeAttributes& obj) {
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
};

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, VertexAttributes, EdgeAttributes> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

int main() {

    std::string path_attributes = "/home/vsicard/Documents/POSTDOC_INSERM/DATA/cattle_data/attributes.csv";
    std::string path_movements = "/home/vsicard/Documents/POSTDOC_INSERM/DATA/cattle_data/movements.csv";

    // Read the CSV files
    std::ifstream movements_file(path_movements);
    if(!movements_file.good()) {
        std::cout << "ERROR file 'movements.csv' not exists" << std::endl;
    } else {
        std::cout << "movements.csv OK" << std::endl;
    }
    std::ifstream attributes_file(path_attributes);
    if(!attributes_file.good()) {
        std::cout << "ERROR file 'attributes.csv' not exists" << std::endl;
    } else {
        std::cout << "attributes.csv loaded" << std::endl;
    }

    // csv file
    std::ofstream outputFile("toy_results.csv");
    outputFile << "t,S,I,R" << std::endl;

    // Create empty graph
    Graph G;

    // Create nodes
    std::cout << "Create graph G" << std::endl;
    std::string attributes_header;
    std::getline(attributes_file, attributes_header); // Skip the header line
    std::string attributes_line;

    std::unordered_map<int, Vertex> idToVertex;

    std::map<std::string, int> mAttributeNameIndex;
    std::istringstream issAttribute(attributes_header);
    std::string elementAttribute;
    int indexAttribute = 0;
    while(std::getline(issAttribute, elementAttribute, ',')) {
        mAttributeNameIndex[elementAttribute] = indexAttribute;
        indexAttribute++;
    }

    std::cout << "Attributes construction ->" << std::endl;

    // lines counter
    std::ifstream attrTmpFile(path_attributes);
    int lineAttributesCount = std::count(std::istreambuf_iterator<char>(attrTmpFile), std::istreambuf_iterator<char>(), '\n')-1;
    attrTmpFile.close();
    std::cout << "lines: " << lineAttributesCount << std::endl;
    // construct progressbar for nodes construction
    progressbar barAttributes(lineAttributesCount);
    // start timer
    auto startAttributes = std::chrono::high_resolution_clock::now();
    // loop reading the file
    while (std::getline(attributes_file, attributes_line)) {
        // update progressbar
//        barAttributes.update();
        std::vector<std::string> attributes_tokens;
        boost::split(attributes_tokens, attributes_line, boost::is_any_of(","));
        std::string node = convertToString(attributes_tokens[mAttributeNameIndex["node"]]);

        VertexAttributes attr;
        for(const auto& pair : mAttributeNameIndex){
            attr.addAttributes(pair.first, convertToString(attributes_tokens[pair.second]));
        }
        attr.addAttributes("state", "S");

        Vertex v = boost::add_vertex(VertexAttributes(attr), G);
        idToVertex[std::stoul(attr.attributes.at("ID"))] = v;
    }
    // print outputs
    std::cout << "\nNumber of nodes: " << boost::num_vertices(G) << std::endl;
    auto endAttributes = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationAttributes = endAttributes-startAttributes;
    std::cout << "Execution time: " << durationAttributes.count() << "s" << std::endl;
    std::cout << "Graph nodes with attributes done" << std::endl;
    // close file
    attributes_file.close();

    // Add edges with attributes
    std::string movements_header;
    std::getline(movements_file, movements_header); // Skip the header line
    std::string movements_line;

    std::map<std::string, int> mMovementsNameIndex;
    std::istringstream issMovement(movements_header);
    std::string elementMovement;
    int indexMovement = 0;
    while(std::getline(issMovement, elementMovement, ',')) {
        mMovementsNameIndex[elementMovement] = indexMovement;
        indexMovement++;
    }

    std::cout << "Movements construction" << std::endl;
    // lines counter
    std::ifstream moveTmpFile(path_movements);
    int lineMovementsCount = std::count(std::istreambuf_iterator<char>(moveTmpFile), std::istreambuf_iterator<char>(), '\n')-1;
    attrTmpFile.close();
    std::cout << "lines: " << lineMovementsCount << std::endl;
    // construct progressbar for nodes construction
    progressbar barMovement(lineMovementsCount);
    auto startMovements = std::chrono::high_resolution_clock::now();
    while (std::getline(movements_file, movements_line)) {
//        barMovement.update();
        std::vector<std::string> movements_tokens;
        boost::split(movements_tokens, movements_line, boost::is_any_of(","));

        EdgeAttributes attr;
        for(const auto& pair : mMovementsNameIndex){
            attr.addAttribute(pair.first, convertToString(movements_tokens[pair.second]));
        }

        Vertex v1;
        v1 = idToVertex.at(std::stoul(attr.attributes.at("i")));
        Vertex v2;
        v2 = idToVertex.at(std::stoul(attr.attributes.at("j")));

        const std::type_info& type = typeid(v1);
        boost::add_edge(v1, v2, EdgeAttributes(attr), G);
    }
    std::cout << "\nNumber of edges: " << boost::num_edges(G) << std::endl;
    auto endMovements = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationMovements = endMovements-startMovements;
    std::cout << "Execution time: " << durationMovements.count() << "s" << std::endl;
    std::cout << "EDGES done" << std::endl;

    // close file
    movements_file.close();

    // Randomly select a node
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> dist(0, boost::num_vertices(G) - 1);
    boost::graph_traits<Graph>::vertex_descriptor randomVertex = boost::vertex(dist(rng), G);

    VertexAttributes* randomVertexAttributes = &G[randomVertex];
    std::cout << "State: " << randomVertexAttributes->getAttribute("state") << std::endl;
    randomVertexAttributes->addAttributes("state", "I");
    std::cout << "State: " << randomVertexAttributes->getAttribute("state") << std::endl;

    // SIR
    int max_time = 2556;
    double beta = 0.5;
    double gamma = 0.1;

    auto start = std::chrono::high_resolution_clock::now();
    progressbar bar(max_time);
    for (int t = 0; t < max_time; ++t) {
        bar.update();
//        std::cout << "T=" << t << std::endl;
        std::vector<Vertex> S;
        std::vector<Vertex> I;
        std::vector<Vertex> R;
        boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = boost::vertices(G); vi != vi_end; ++vi) {
            Vertex node = *vi;
            VertexAttributes* nodeAttributes = &G[node];
            if (nodeAttributes->getAttribute("state") == "I") {
                boost::graph_traits<Graph>::out_edge_iterator ei, ei_end;
                for(boost::tie(ei, ei_end) = boost::out_edges(node, G); ei != ei_end; ++ei) {
                    Edge edge = *ei;
                    EdgeAttributes* edgeAttributes = &G[edge];
                    int time =  std::stoul(edgeAttributes->getAttribute("t"));

                    if(time == t) {
                        std::cout << "t: " << edgeAttributes->getAttribute("t") << " i: " << edgeAttributes->getAttribute("i") << " j: " << edgeAttributes->getAttribute("j") << " w: " << edgeAttributes->getAttribute("w") << std::endl;
                        boost::graph_traits<Graph>::adjacency_iterator ai, ai_end;
                        for (boost::tie(ai, ai_end) = boost::adjacent_vertices(node, G); ai != ai_end; ++ai) {
                            Vertex neighbor = *ai;
                            VertexAttributes* neighborAttributes = &G[neighbor];
                            if (neighborAttributes->getAttribute("state") == "S") {
                                std::bernoulli_distribution bernoulliDistBeta(beta);
                                if (bernoulliDistBeta(rng)) {
                                    I.push_back(neighbor);
                                }
                            }
                        }
                    }
                }

                std::bernoulli_distribution bernoulliDistGamma(gamma);
                if (bernoulliDistGamma(rng)) {
                    R.push_back(node);
                } else {
                    I.push_back(node);
                }
            }
            if(nodeAttributes->getAttribute("state") == "S") {
                S.push_back(node);
            }
        }

        // Update states
        for (Vertex& node : I) {
            VertexAttributes* nodeAttributes = &G[node];
            nodeAttributes->addAttributes("state", "I");
        }
        for (Vertex& node : R) {
            VertexAttributes* nodeAttributes = &G[node];
            nodeAttributes->addAttributes("state", "R");
        }

        outputFile << t << "," << S.size() << "," << I.size() << "," << R.size() << std::endl;
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationSIR = end-start;
    std::cout << "Execution time: " << durationSIR.count() << "s" << std::endl;

    outputFile.close();

//    // Générer le fichier DOT
//    std::ofstream dotFile("graph.dot");
//    boost::write_graphviz(dotFile, G);
//    dotFile.close();
//
//    // Appeler la commande Graphviz pour convertir le fichier DOT en une image JPEG
//    std::system("dot -Tjpeg -o graph.jpg graph.dot");
    return 0;
}
