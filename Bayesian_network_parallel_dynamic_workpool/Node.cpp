#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "Node.h"
#include "method.h"

using namespace std;

Node::Node(){}
Node::Node(string nodeName){
    Name = nodeName;
}
Node::Node(string nodeName, vector<string> data){
    Name = nodeName;
    Data = data;
}
Node::Node(string nodeName,vector<string> data, int id){
    Name = nodeName;
    Data = data;
    Id = id;
}

void Node::addAdjacentNode(const Node& adjacent){
        Adjacents.insert(adjacent);
    }

void Node::addChildrenNode(const Node& child) {
        Childs.insert(child);
    }
void Node::addParentNode(const Node& parent) {
        Parents.insert(parent);
    }
void Node::printRelation() const {
    for(const auto& child : Childs){
        cout<<Name<<" --> "<<child.Name<<endl;
    }
}

bool Node::operator<(const Node& other) const{
    return Id < other.Id;
}

bool Node:: operator==(const Node& other) const {
        return Id == other.Id && Name == other.Name;
    }



unordered_map<string, double> Node:: probabilityTable() const{
    unordered_map<string, double> table;
    for(const auto& value:  Data){
            table[value] += 1;
        }
        for(auto& entry: table){
            entry.second /= Data.size();
        }
        return table;
}

unordered_map<string,double> Node:: bayesProbability(const vector<Node> conditional) const{
        unordered_map<string, double> jointDistribution;
        unordered_map<string, double> conditionalDistribution;

        // Gộp tất cả các node vào một vector để tính phân phối xác suất
        vector<Node> allNodes = conditional;
        allNodes.push_back(*this);

        // Tính xác suất chung P(startNode, endNode, condition)
        jointDistribution = ::probabilityDistribution(allNodes);

        // Tính xác suất biên P(condition)
        conditionalDistribution = ::probabilityDistribution(conditional);

        // Tính P(Node| condition)
        unordered_map<string, double> bayesTable;
        for (const auto& entry : jointDistribution) {
            string key = entry.first;

            // Tạo key chỉ bao gồm các giá trị của "conditional"
            stringstream ss(key);
            string token, conditionKey = "";
            vector<string> tokens;

            // Phân tách key bằng dấu phẩy
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            // Lấy các giá trị condition từ tokens
            for (size_t i = 0; i < conditional.size(); ++i) {
                conditionKey += tokens[i] + ",";
            }
            conditionKey.pop_back(); // Xóa dấu phẩy cuối

            // Tính P(startNode, endNode | condition)
            if (conditionalDistribution.find(conditionKey) != conditionalDistribution.end()) {
                bayesTable[key] = entry.second / conditionalDistribution[conditionKey];
            }
        }

        return bayesTable;
}