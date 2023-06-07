cmake_minimum_required(VERSION 3.25)
project(CPP_optimize)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPP_optimize main.cpp serialize_graph.h VertexAttributes.cpp VertexAttributes.h EdgeAttributes.cpp EdgeAttributes.h AttributeException.cpp AttributeException.h)
