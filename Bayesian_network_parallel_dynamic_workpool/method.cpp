#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <omp.h>
#include "Node.h"
#include "method.h"
#include "Edge.h"
using namespace std;

class Node;

unordered_map<string, double> probabilityDistribution(const vector <Node>& nodes) {
    omp_set_num_threads(2);
        unordered_map<string, double> distribution;

        // Khởi tạo số đếm cho mỗi trường hợp
        size_t size = nodes[0].Data.size();
        
        for (const auto& node : nodes) {
            if (node.Data.size() != size) {
                cerr << "All nodes must have the same size of data!" << endl;
                return {};
            }
        }

        // Tạo biến cho các chỉ số
        vector<string> values;

        #pragma omp parallel
        {
            unordered_map<string, size_t> local_distribution;

            #pragma omp for
            for (size_t i = 0; i < nodes.size(); ++i) {
                string key = "";
                for (const auto& node : nodes) {
                    key += node.Data[i] + ",";
                }
                key.pop_back(); // Xóa dấu phẩy cuối
                local_distribution[key]++;
            
            }
            #pragma omp critical
            {
                for (const auto& entry : local_distribution) {
                    distribution[entry.first] += entry.second;
                }
            }

        }

        // Đếm số lượng trường hợp
        for (size_t i = 0; i < size; ++i) {
            string key = "";
            for (const auto& node : nodes) {
                key += node.Data[i] + ",";
            }
            key.pop_back(); // Xóa dấu phẩy cuối
            distribution[key]++;
        }

        // Tính xác suất cho từng trường hợp
        for (auto& entry : distribution) {
            entry.second /= size; // Chia cho tổng số dữ liệu
        }

        return distribution;
    }

    pair<bool, vector<Node>> CI_test(const vector<Node>& Nodes, Edge edge, int degree, double threshold){
        int n = Nodes.size();
        vector<bool> select(n, false);
        fill(select.begin(), select.begin() + degree, true);

        do {
            vector<Node> conditional;
            for (int i  = 0 ; i < n ; ++i){
                if(select[i]){
                    // them vao vector dieu kien conditional
                    conditional.push_back(Nodes[i]);
                }
            }
            if (edge.mutual_information_test(conditional) < threshold){
                return {true,conditional};
            }
        } while (prev_permutation(select.begin(), select.end()));

        return {false, {}};
    }

