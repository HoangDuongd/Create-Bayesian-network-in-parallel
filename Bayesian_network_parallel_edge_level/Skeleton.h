#pragma once
#ifndef SKELETON_H
#define SKELETON_H
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include <map>
#include <set>

class Skeleton{
    public:
    std::vector<Edge> edges;
    std::vector<Node> nodes;
    std::map<std::vector<Node>,std::vector<Node>> sepset;
    Skeleton(std::vector<Edge> Edges);
    Skeleton(std::vector<Edge> Edges, std::vector<Node> Nodes);
    void find_Skeleton();
    void find_DAG();
    // bool is_cycle(Node nod );
};


#endif // SKELETON_H
