#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include <set>
#include <algorithm>
#include <omp.h>
#include "Node.h"
#include "method.h"
#include "Skeleton.h"



using namespace std;

Skeleton::Skeleton(vector<Edge> Edges){
    edges = Edges;
    set<Node> nods;
    for(size_t i = 0 ; i < edges.size(); i++){
        nods.insert(edges[i].NodeStart);
        nods.insert(edges[i].NodeEnd);
    }
    nodes = vector<Node>(nods.begin(), nods.end());
}

Skeleton::Skeleton(vector<Edge> Edges, vector<Node> Nodes){
    edges = Edges;
    nodes = Nodes;
}

void Skeleton::find_Skeleton(){
    set<Node> uniqueNodes;
    set<Edge> eraseEdges; 
    int i = 1;
    omp_set_num_threads(3);
        for (const auto& edge : edges){ // tạo set uniqueNodes chứa toàn bộ các nodes
                uniqueNodes.insert(edge.NodeStart);
                uniqueNodes.insert(edge.NodeEnd);
        }
    
    for (const auto& node : nodes){ // thêm node vào set uniqueNodes trên nếu nodes đó không thuộc đồ thị
        uniqueNodes.insert(node);
    }
    #pragma omp parallel for schedule(dynamic)
    for( auto& edge : edges){ //const auto& edge : edges
        set<Node> conditional(uniqueNodes);
        conditional.erase(edge.NodeStart);
        conditional.erase(edge.NodeEnd);
        vector<Node> conditional_CI_test(conditional.begin(),conditional.end());
        cout<<"calculate "<<i<<"/"<<edges.size()<<endl;
        i++;
        for(int i = 0 ; i < conditional_CI_test.size() ; i++){
            
            auto result = CI_test(conditional_CI_test,edge,i,0.00007);

            if(result.first){
                eraseEdges.insert(edge);
                set<Node> pair_of_nodes = {edge.NodeStart,edge.NodeEnd};
                vector<Node> pair_of_nods(pair_of_nodes.begin(),pair_of_nodes.end());
                set<Node> sepset_of_2node(result.second.begin(), result.second.end());
                vector<Node> sepset_of_2nods(sepset_of_2node.begin(),sepset_of_2node.end());
                sepset[pair_of_nods] = sepset_of_2nods;
                break;
            }
        }
    }
    // xóa cạnh độc lập có điều kiện
    edges.erase(
    remove_if(edges.begin(), edges.end(), [&eraseEdges](const Edge& edge) {
        return eraseEdges.find(edge) != eraseEdges.end();
    }),
    edges.end()
    );
    // xóa và chỉ để lại những node không được liên kết với đồ thị
    set<Node> eraseNodes;
    for (const auto& edge : edges){
        eraseNodes.insert(edge.NodeStart);
        eraseNodes.insert(edge.NodeEnd);
    }

    for (auto it = uniqueNodes.begin(); it != uniqueNodes.end(); ) {
        if (eraseNodes.find(*it) != eraseNodes.end()) {
            it = uniqueNodes.erase(it);  // Nếu tìm thấy, xoá phần tử và chuyển sang phần tử tiếp theo
        } else {
            ++it;  // Nếu không tìm thấy, chỉ cần chuyển đến phần tử tiếp theo
        }
    }

    nodes.insert(nodes.end(), uniqueNodes.begin(), uniqueNodes.end());
    // tạo danh sách nodes kề của 1 node

    for(size_t i = 0 ; i < nodes.size(); i++){
        for(size_t j = 0 ; j < edges.size(); j++){
            if(nodes[i] == edges[j].NodeStart){
                nodes[i].addAdjacentNode(edges[j].NodeEnd);
            }
            if(nodes[i] == edges[j].NodeEnd){
                nodes[i].addAdjacentNode(edges[j].NodeStart);
            }
        }
    }

}

void Skeleton::find_DAG(){    
    // với mọi đỉnh, nếu đỉnh đó có số đỉnh kề >= 2 
    for (size_t i = 0 ; i < nodes.size() ; i++){
        cout<<"xet dinh thu "<<i<<": "<<nodes[i].Name<<endl;//////////////////////////////////////////////////
        if(nodes[i].Adjacents.size() >= 2){        
            // lấy tập 2 đỉnh khác nhau trong tập đỉnh kề
            cout<<"co hon hoac bang 2 dinh ke"<<endl;
            auto& adj_nodes = nodes[i].Adjacents;
            for (auto it1 = adj_nodes.begin(); it1 != adj_nodes.end(); ++it1) {
                // Lấy các phần tử tiếp theo để tránh trùng lặp
                for (auto it2 = std::next(it1); it2 != adj_nodes.end(); ++it2) {
                    cout<<it1->Name<<" "<<it2->Name<<endl;///////////////////////////////
                    set<Node> pair_of_nodes = {*it1,*it2};
                    vector<Node> pair_of_nods(pair_of_nodes.begin(), pair_of_nodes.end());

                    if(find(sepset[pair_of_nods].begin(),sepset[pair_of_nods].end(), nodes[i]) == sepset[pair_of_nods].end()){
                        cout<<"ko thuoc sepset"<<endl;
                        for(size_t j = 0 ; j < nodes.size(); j++){
                            if(nodes[j] == *it1 && j != i && nodes[j] < nodes[i] ){
                                nodes[j].addChildrenNode(nodes[i]);
                            }else if (nodes[j] == *it2 && j != i && nodes[j] < nodes[i]) {
                                nodes[j].addChildrenNode(nodes[i]);
                            }

                        }
                    }
                }
            }
        }
    }
}

// bool is_cycle(Node nod){
//     if(nod.Childs.size() != 0){
//         return false;
//     } else{
//         for (auto& element : Childs){
//             is_cycle(element);
//         }
//     }
// }