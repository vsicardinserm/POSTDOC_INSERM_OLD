#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/graph/graphviz.hpp>
#include "EdgeAttributes.h"
#include "VertexAttributes.h"
#include "serialize_graph.h"
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#include <unordered_map>
#include <map>
#include <random>
#include "progressbar.hpp"
#include <chrono>

std::string convertToString(const std::string& attribute) {
    return attribute;
}
std::string convertToString(int attribute) {
    return std::to_string(attribute);
}

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, VertexAttributes, EdgeAttributes> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

int main() {
    std::cout << "\t\033[1;33m";
    std::cout << R"(

  ______       _     _                _       _             _           _   _______                                   _   _   _      _                      _
 |  ____|     (_)   | |              (_)     | |           (_)         | | |__   __|                                 | | | \ | |    | |                    | |
 | |__   _ __  _  __| | ___ _ __ ___  _  ___ | | ___   __ _ _  ___ __ _| |    | | ___ _ __ ___  _ __   ___  _ __ __ _| | |  \| | ___| |___      _____  _ __| | __
 |  __| | '_ \| |/ _` |/ _ \ '_ ` _ \| |/ _ \| |/ _ \ / _` | |/ __/ _` | |    | |/ _ \ '_ ` _ \| '_ \ / _ \| '__/ _` | | | . ` |/ _ \ __\ \ /\ / / _ \| '__| |/ /
 | |____| |_) | | (_| |  __/ | | | | | | (_) | | (_) | (_| | | (_| (_| | |    | |  __/ | | | | | |_) | (_) | | | (_| | | | |\  |  __/ |_ \ V  V / (_) | |  |   <
 |______| .__/|_|\__,_|\___|_| |_| |_|_|\___/|_|\___/ \__, |_|\___\__,_|_|    |_|\___|_| |_| |_| .__/ \___/|_|  \__,_|_| |_| \_|\___|\__| \_/\_/ \___/|_|  |_|\_\
        | |                                            __/ |                                   | |
        |_|                                           |___/                                    |_|

)" << std::endl;
    std::cout << "\t\033[0m" << std::endl;

    std::string path_attributes = "/home/vsicard/Documents/POSTDOC_INSERM/DATA/cattle_data/attributes.csv";
    std::string path_movements = "/home/vsicard/Documents/POSTDOC_INSERM/DATA/cattle_data/movements.csv";

    // Read the CSV files
    std::ifstream movements_file(path_movements);
    if(!movements_file.good()) {
        std::cout << "\t\033[1;31mERROR file 'movements.csv' not exists\033[0m" << std::endl;
    } else {
        std::cout << "\tmovements.csv \033[0;32mLOADED\033[0m" << std::endl;
    }
    std::ifstream attributes_file(path_attributes);
    if(!attributes_file.good()) {
        std::cout << "\t\033[1;31mERROR file 'attributes.csv' not exists\033[0m" << std::endl;
    } else {
        std::cout << "\tattributes.csv \033[0;32mLOADED\033[0m" << std::endl;
    }

    // csv file
    std::ofstream outputFile("toy_results.csv");
    outputFile << "t,S,I,R" << std::endl;

    // Create empty graph
    Graph G;

    // Create nodes
    std::cout << "\n\t\033[1;33mCreate graph G\033[0m" << std::endl;
    std::cout << "\t\033[1;33m---------------\033[0m" << std::endl;

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

    std::cout << "\t\t\033[0;34mAttributes construction:\033[0m" << std::endl;

    // lines counter
    std::ifstream attrTmpFile(path_attributes);
    int lineAttributesCount = std::count(std::istreambuf_iterator<char>(attrTmpFile), std::istreambuf_iterator<char>(), '\n')-1;
    attrTmpFile.close();
    std::cout << "\t\t\tnb lines: " << lineAttributesCount << std::endl;
    // construct progressbar for nodes construction
    int divider = lineAttributesCount/100;
    progressbar barAttributes(lineAttributesCount/divider);
    // start timer
    auto startAttributes = std::chrono::high_resolution_clock::now();
    int cptAttr = 0;
    // loop reading the file
    while (std::getline(attributes_file, attributes_line)) {
        // update progressbar
        if(cptAttr % divider == 0) {
            barAttributes.update();
        }
        ++cptAttr;
        std::vector<std::string> attributes_tokens;
        boost::split(attributes_tokens, attributes_line, boost::is_any_of(","));
        std::string node = convertToString(attributes_tokens[mAttributeNameIndex["node"]]);

        VertexAttributes attr;
        for(const auto& pair : mAttributeNameIndex){
            attr.addAttributes(pair.first, convertToString(attributes_tokens[pair.second]));
        }
        attr.addAttributes("state", "S");

        Vertex v = boost::add_vertex(VertexAttributes(attr), G);
        idToVertex[std::stoul(attr.getAttribute("ID"))] = v;
    }
    // print outputs
    std::cout << "\n\t\t\tNumber of nodes: " << boost::num_vertices(G) << std::endl;
    auto endAttributes = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationAttributes = endAttributes-startAttributes;
    std::cout << "\t\t\tExecution time: " << durationAttributes.count() << "s" << std::endl;
    std::cout << "\t\t\tGraph nodes with attributes \033[0;32mDONE\033[0m" << std::endl;
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

    std::cout << "\t\t\033[0;34mMovements construction:\033[0m" << std::endl;
    // lines counter
    std::ifstream moveTmpFile(path_movements);
    int lineMovementsCount = std::count(std::istreambuf_iterator<char>(moveTmpFile), std::istreambuf_iterator<char>(), '\n')-1;
    attrTmpFile.close();
    std::cout << "\t\t\tnb lines: " << lineMovementsCount << std::endl;
    // construct progressbar for nodes construction
    divider = lineMovementsCount/100;
    progressbar barMovement(lineMovementsCount/divider);
    int cptMvt = 0;
    auto startMovements = std::chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        std::string movements_line;
        int local_cptMvt = 0;
        int cptWeek = 0;
        std::vector<std::string> local_movements_tokens;
        EdgeAttributes local_attr;
        Vertex local_v1, local_v2;

        // Chaque thread lit une ligne du fichier et traite les données correspondantes
        while (true) {
            std::string local_movements_line;
            #pragma omp critical (file_read)
            {
                if (!std::getline(movements_file, local_movements_line)) {
                    // Fin du fichier, sortie de la boucle
                    break;
                }
            }

            if (local_cptMvt % divider == 0) {
            #pragma omp critical (progress_update)
                {
                    barMovement.update();
                }
            }
            ++local_cptMvt;

            boost::split(local_movements_tokens, local_movements_line, boost::is_any_of(","));

            for (const auto& pair : mMovementsNameIndex) {
                local_attr.addAttribute(pair.first, convertToString(local_movements_tokens[pair.second]));
            }

            local_v1 = idToVertex.at(std::stoul(local_attr.getAttribute("i")));
            local_v2 = idToVertex.at(std::stoul(local_attr.getAttribute("j")));

            // grouping per week
            #pragma omp critical (week_grouping)
            if(std::stoul(local_attr.getAttribute("t")) % 7 == 0) {
                cptWeek +=1;
            }
            local_attr.addAttribute("w", convertToString(cptWeek));

            // Section critique pour éviter les conflits lors de l'ajout d'arêtes au graphe
            #pragma omp critical (graph_update)
            {
                const std::type_info& type = typeid(local_v1);
                boost::add_edge(local_v1, local_v2, EdgeAttributes(local_attr), G);
            }
        }
    }

    std::cout << "\n\t\t\tNumber of edges: " << boost::num_edges(G) << std::endl;
    auto endMovements = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationMovements = endMovements-startMovements;
    std::cout << "\t\t\tExecution time: " << durationMovements.count() << "s" << std::endl;
    std::cout << "\t\t\tGraph weighted edges \033[0;32mDONE\033[0m" << std::endl;

    // close file
    movements_file.close();

    // Randomly select a node
    std::random_device rd;
    std::mt19937 rng(rd());
    int nb_I = 3;
    for (int ii = 0; ii < nb_I;++ii) {
        std::uniform_int_distribution<> dist(0, boost::num_vertices(G) - 1);
        boost::graph_traits<Graph>::vertex_descriptor randomVertex = boost::vertex(dist(rng), G);

        VertexAttributes *randomVertexAttributes = &G[randomVertex];
        randomVertexAttributes->addAttributes("state", "I");
    }

    // SIR
    int max_time = 2556;
    double beta = 0.5;
    double gamma = 0.1;

    auto start = std::chrono::high_resolution_clock::now();
    divider = max_time/100;
    progressbar bar(max_time/divider);
    std::cout << "\n\t\033[1;33mStart SIR loop\033[0m \033[3m(" << max_time << " iteration)\033[0m" << std::endl;
    std::cout << "\t\033[1;33m-------------\033[0m" << std::endl;
    #pragma omp parallel for
    for (int t = 0; t < max_time; ++t) {
        if(t % divider == 0) {
            #pragma omp critical
            bar.update();
        }
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
//                        #pragma omp critical
//                        std::cout << "\tt: " << edgeAttributes->getAttribute("t") << " i: " << edgeAttributes->getAttribute("i") << " j: " << edgeAttributes->getAttribute("j") << " w: " << edgeAttributes->getAttribute("w") << std::endl;
                        boost::graph_traits<Graph>::adjacency_iterator ai, ai_end;
                        for (boost::tie(ai, ai_end) = boost::adjacent_vertices(node, G); ai != ai_end; ++ai) {
                            Vertex neighbor = *ai;
                            VertexAttributes* neighborAttributes = &G[neighbor];
                            if (neighborAttributes->getAttribute("state") == "S") {
                                std::bernoulli_distribution bernoulliDistBeta(beta);
                                if (bernoulliDistBeta(rng)) {
                                    #pragma omp critical
                                    I.push_back(neighbor);
                                }
                            }
                        }
                    }
                }

                std::bernoulli_distribution bernoulliDistGamma(gamma);
                if (bernoulliDistGamma(rng)) {
                    #pragma omp critical
                    R.push_back(node);
                } else {
                    #pragma omp critical
                    I.push_back(node);
                }
            }
            if(nodeAttributes->getAttribute("state") == "S") {
                #pragma omp critical
                S.push_back(node);
            } else if(nodeAttributes->getAttribute("state") == "R") {
                #pragma omp critical
                R.push_back(node);
            }
        }

        // Update states
        #pragma omp parallel for
        for (int i = 0; i < I.size(); ++i) {
            Vertex node = I[i];
            VertexAttributes* nodeAttributes = &G[node];
            #pragma omp critical
            nodeAttributes->addAttributes("state", "I");
        }

        #pragma omp parallel for
        for (int i = 0; i < R.size(); ++i) {
            Vertex node = R[i];
            VertexAttributes* nodeAttributes = &G[node];
            #pragma omp critical
            nodeAttributes->addAttributes("state", "R");
        }

        #pragma omp critical
        outputFile << t << "," << S.size() << "," << I.size() << "," << R.size() << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationSIR = end-start;
    std::cout << "\t\nExecution time: " << durationSIR.count() << "s" << std::endl;

    outputFile.close();

    std::cout << "\t\n\033[0;33mExecution finished\033[0m" << std::endl;

//    // Générer le fichier DOT
//    std::ofstream dotFile("graph.dot");
//    boost::write_graphviz(dotFile, G);
//    dotFile.close();
//
//    // Appeler la commande Graphviz pour convertir le fichier DOT en une image JPEG
//    std::system("dot -Tjpeg -o graph.jpg graph.dot");
    return 0;
}
