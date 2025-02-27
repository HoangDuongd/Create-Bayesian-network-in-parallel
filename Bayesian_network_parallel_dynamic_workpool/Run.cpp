#include <iostream>
#include <fstream>
#include <vector>
#include "readFile.h"
#include "Node.h"
#include "Edge.h"
#include "Skeleton.h"
#include "method.h"
#include <unordered_map>
#include <sstream>
#include <chrono>
#include <omp.h>

using namespace std;


int main(){
    // Đọc file
    omp_set_num_threads(16);
    // string filename = "asia10K.csv";
    // string filename = "alarm_s5000.txt";
    string filename = "Data/asia10k.csv";
    // string filename = "survey-lung-cancer.csv";
    vector<vector<string>> data = readCSV(filename);
    cout<<"done"<<endl;
    vector<Node> Nodes;

    // tạo nodes từ dữ liệu
    for (size_t i = 0 ; i < data[0].size(); i++){
        vector<string> values;
        for(size_t j = 1; j < data.size(); j++){
            values.push_back(data[j][i]);
        }
        Node N(data[0][i], values, i);
        Nodes.emplace_back(N);
    }
    cout<<"done"<<endl;
    // tạo cạnh từ các nodes
    vector<Edge> Edges;
    for(size_t i = 0 ; i < Nodes.size() - 1; i++){
        for(size_t j = i + 1; j < Nodes.size(); j++){
            Edge a =  Edge(Nodes[i], Nodes[j]);
            Edges.push_back(a);
            // a.printEdge();
        }
    }
    cout<<"done"<<endl;


    // for (const auto& nod : Nodes){
    //     cout<<nod.Name<<" ";
    // }

    // cout<<endl;

    // for (const auto& nod : Nodes){
    //     unordered_map<string, double> table = nod.probabilityTable();
    //     cout<<nod.Name<<endl;
    //     for (const auto& entry : table){
    //         cout<<entry.first<<" "<<entry.second<<endl;
    //     }
    // }


    /*
        tính xác suất bayes với 1 đỉnh
    */

    // for (const auto& nod : Nodes){
    //     vector<Node> condition(Nodes.begin(), Nodes.end());
    //     condition.erase(std::remove(condition.begin(), condition.end(), nod), condition.end());
    //     unordered_map<string, double> bayesTable = nod.bayesProbability(condition);

    //     cout<<"bayes Probability of "<<nod.Name<<" | ";
    //     for (const auto& element : condition){
    //         cout<<element.Name<<" ";
    //     }
    //     cout<<endl;

    //     for (const auto& entry : bayesTable){
    //         cout<<entry.first<<"---"<<entry.second<<endl;
    //     }
    // }

    /*
        tính xác suất Bayes với 2 đỉnh
    */

    // for (size_t i = 0 ; i < Edges.size(); i++ ){
    //     vector<Node> condition(Nodes.begin(), Nodes.end());
    //     condition.erase(std::remove(condition.begin(), condition.end(), Edges[i].NodeStart), condition.end());
    //     condition.erase(std::remove(condition.begin(),condition.end(),Edges[i].NodeEnd), condition.end());
    //     unordered_map<string,double> bayesTable = Edges[i].bayesProbability(condition);
    //     for (const auto& entry : bayesTable){
    //         cout<<entry.first<<":"<<entry.second<<endl;
    //     }
    //     cout<<Edges[i].NodeStart.Name <<"with"<<Edges[i].NodeEnd.Name<<endl;
    // }

    /*
        Tạo skeleton từ các cạnh
    */
///////////////////////////////////////////////////////////////
    auto start = chrono::high_resolution_clock::now(); 

    Skeleton skeleton1(Edges);

    skeleton1.find_Skeleton();

    for (const auto& edge : skeleton1.edges){
        edge.printEdge();
    }
    cout<<"done"<<endl;
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time: " << elapsed.count() << " second" << endl;
    // cout<<"DAG:"<<endl;

    skeleton1.find_DAG();
    cout<<"--------------";
    for(size_t i = 0 ; i < skeleton1.nodes.size(); i++){
        skeleton1.nodes[i].printRelation();
    }
/////////////////////////////////////////////////////////////


    // for (size_t i = 0 ; i < skeleton1.edges.size(); i++){
    //     for(const auto& adj : skeleton1.edges[i].NodeStart.Adjacents){
    //         cout<<skeleton1.edges[i].NodeStart.Name<<adj.Name<<endl;
    //     }
    //     cout<<"-----"<<endl;
    // }

    // for(size_t i = 0 ; i < skeleton1.nodes.size(); i++){
    //     cout<<skeleton1.nodes[i].Name<<endl;
    //     cout<<skeleton1.nodes[i].Adjacents.size()<<endl;

    // }
    // for(size_t i = 0 ; i < skeleton1.edges.size(); i++){
    //     cout<<skeleton1.edges[i].NodeStart.Adjacents.size();
    //     cout<<skeleton1.edges[i].NodeEnd.Adjacents.size();
    // }
    return 0;
}