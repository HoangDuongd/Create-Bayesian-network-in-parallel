#pragma once
#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "Node.h"

class Node;

class Edge{
public:
    Node NodeStart;
    Node NodeEnd;
    int Id;

    Edge(Node start, Node end); // Khởi tạo với 2 nút (bắt đầu và kết thúc)

    void printEdge() const; // In thông tin cạnh
    bool operator<(const Edge& other) const;
    bool operator==(const Edge& other)const;

    std::unordered_map<std::string, double> bayesProbability(const std::vector<Node>& condition) const;

    double mutual_information_test(const std::vector<Node>& conditional);

    double calculate_mi_without_condition();

    double calculate_mi_with_condition(const std::vector<Node>& conditional);

};


#endif // EDGE_H
