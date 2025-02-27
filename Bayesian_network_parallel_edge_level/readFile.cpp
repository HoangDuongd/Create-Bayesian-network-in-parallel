#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;
vector<vector<string>> readCSV(const string& filename){

    ifstream inputFile(filename);
    vector<vector<string>> data;

    if(!inputFile.is_open()){
        cerr <<"Could not read "<<endl;
        return data;
    }
    string line, value;
    while(getline(inputFile, line)) {
        vector<string> row;
        stringstream ss(line);
        while(getline(ss, value,',')){
            row.push_back(value);
        }
        data.push_back(row);
    }

    inputFile.close();
    return data;
}