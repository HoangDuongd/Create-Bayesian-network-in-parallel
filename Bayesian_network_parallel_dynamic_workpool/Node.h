#pragma once
#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

class Node{
    public:
        std:: string Name;
        std::set<Node> Parents;
        std::set<Node> Childs;
        std::set<Node> Adjacents;
        std::vector<std::string> Data;
        int Id;

        Node();
        Node(std::string nodeName);
        Node(std::string nodeName, std::vector<std::string> data);
        Node(std::string nodeName, std::vector<std::string> data, int id);
        void addAdjacentNode(const Node& adjacent);
        void addChildrenNode(const Node& child);
        void addParentNode(const Node& parent);
        void printRelation() const;
        bool operator<(const Node& other) const;
        bool operator==(const Node& other) const;

        std::unordered_map<std::string, double> probabilityTable() const;
        std::unordered_map<std::string, double> bayesProbability(const std::vector<Node> condition) const;

};

#endif // NODE_H
