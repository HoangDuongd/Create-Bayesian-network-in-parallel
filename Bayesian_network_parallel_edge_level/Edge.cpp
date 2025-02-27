#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#include "Node.h"
#include "Edge.h"
#include "method.h"

using namespace std;

    Edge::Edge(Node start, Node end){
        NodeStart = start;
        NodeEnd = end;
        string str_id = to_string(NodeStart.Id) + to_string(NodeEnd.Id);
        Id = stoi(str_id);
    }

    void Edge:: printEdge() const {
        cout << NodeStart.Name << " -- " << NodeEnd.Name << endl;
    }

    bool Edge:: operator<(const Edge& other) const{
        return Id < other.Id;
    }

    unordered_map<string, double> Edge:: bayesProbability(const vector<Node>& condition) const {
        unordered_map<string, double> jointDistribution;
        unordered_map<string, double> conditionalDistribution;

        // Gộp tất cả các node vào một vector để tính phân phối xác suất
        vector<Node> allNodes = condition;
        allNodes.push_back(NodeStart);
        allNodes.push_back(NodeEnd);

        // Tính xác suất chung P(startNode, endNode, condition)
        jointDistribution = ::probabilityDistribution(allNodes);

        // Tính xác suất biên P(condition)
        conditionalDistribution = ::probabilityDistribution(condition);

        // Tính P(startNode, endNode | condition)
        unordered_map<string, double> bayesTable;
        for (const auto& entry : jointDistribution) {
            string key = entry.first;

            // Tạo key chỉ bao gồm các giá trị của "condition"
            stringstream ss(key);
            string token, conditionKey = "";
            vector<string> tokens;

            // Phân tách key bằng dấu phẩy
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            // Lấy các giá trị condition từ tokens
            for (size_t i = 0; i < condition.size(); ++i) {
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

    double Edge:: mutual_information_test(const vector<Node>& conditional) {
        // Trường hợp không có điều kiện (conditional vector rỗng)
        if (conditional.empty()) {
            return calculate_mi_without_condition();
        } else {
            return calculate_mi_with_condition(conditional);
        }
    }

    double Edge:: calculate_mi_without_condition() {
        // Giả sử chúng ta tính MI cho một tập hợp các node (ví dụ: từ dữ liệu mẫu)
        double mi_value = 0.0;
        unordered_map<string, double> proX =  NodeStart.probabilityTable();
        unordered_map<string, double> proY = NodeEnd.probabilityTable();
        vector<Node> nodes;
        nodes.push_back(NodeStart);
        nodes.push_back(NodeEnd);
        unordered_map<string, double> distribution = probabilityDistribution(nodes);

        for (const auto& entry : distribution){
            size_t pos = entry.first.find(",");
            string X = entry.first.substr(0,pos);
            string Y = entry.first.substr(pos + 1);
            if (entry.second > 0){
                mi_value += entry.second * log(entry.second / (proX[X] * proY[Y]));
            }
        }
        return mi_value;
    }

    double Edge:: calculate_mi_with_condition(const vector<Node>& conditional) {
        double mi_value = 0.0;
        vector<Node> allNodes = conditional;
        allNodes.push_back(NodeStart);
        allNodes.push_back(NodeEnd);

        // P(all)
        unordered_map<string, double> jointDistribution = :: probabilityDistribution(allNodes);
        //P(A,B|conditional)
        unordered_map<string, double> bayes_all =  this->bayesProbability(conditional);
        //P(A|conditional)
        unordered_map<string, double> bayes_NodeStart = NodeStart.bayesProbability(conditional);
        //P(B|conditional)
        unordered_map<string, double> bayes_NodeEnd = NodeEnd.bayesProbability(conditional);

        for (const auto& entry : jointDistribution){
            string key = entry.first;
            stringstream ss(key);
            string token, bayes_NodeStart_key, bayes_NodeEnd_key;
            vector<string> tokens;

            while (getline(ss, token, ',')){
                tokens.push_back(token);
            }

            // tạo key cho bayes_NodeStart
            for (size_t i = 0 ; i < conditional.size() + 1; i++){
                bayes_NodeStart_key += tokens[i] + ",";
            }
            bayes_NodeStart_key.pop_back();
            //tạo key cho bayes_NodeEnd
            for (size_t i = 0 ; i < conditional.size(); i++){
                bayes_NodeEnd_key += tokens[i] + ",";
            }
            bayes_NodeEnd_key += tokens[tokens.size() - 1];

            if((bayes_NodeStart.find(bayes_NodeStart_key) != bayes_NodeStart.end()) && (bayes_NodeEnd.find(bayes_NodeEnd_key) != bayes_NodeEnd.end())){
                mi_value += entry.second * log (bayes_all[key] / (bayes_NodeStart[bayes_NodeStart_key] * bayes_NodeEnd[bayes_NodeEnd_key]));
            }
        }
       

        return mi_value;
    }