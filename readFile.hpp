#ifndef __READFILE_HPP__
#define __READFILE_HPP__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

using namespace std;


vector<int> getNodes(string dir) {
    string line;
    vector<int> nodes;

    ifstream file(dir);

    while(getline(file, line)) {
        if(line == "TOUR_SECTION") {
            while(getline(file, line)) {
                if(line == "-1") break;
                int id;
                sscanf(line.c_str(), "%d", &id);
                nodes.push_back(id-1);
            }
        }
    }

    return nodes;
}

vector<pair<int, int>> readFile(string dir) {
    vector<pair<int, int>> cities;
    string line;

    ifstream file(dir);

    while(getline(file, line)) {
        if(line == "NODE_COORD_SECTION") {
            while(getline(file, line)) {
                if(line == "EOF") break;
                int id, x, y;
                sscanf(line.c_str(), "%d %d %d", &id, &x, &y);
                cities.push_back(make_pair(x, y));
            }
        }
    }
    return cities;
}

vector<vector<int>> getDistances(vector<pair<int, int>> cities) {
    // cacl ecuclidean distance between every city and store it in a 2d arr
    vector<vector<int>> distances(cities.size(), vector<int>(cities.size()));
    for(int i = 0; i < cities.size(); ++i) {
        for(int j = 0; j < cities.size(); ++j) {
            int x = cities[i].first - cities[j].first;
            int y = cities[i].second - cities[j].second;
            distances[i][j] = round(sqrt(x*x + y*y));
        }
    }

    return distances;
}

void writeToFile(vector<int> path, string filename) {
    string dir = "./" + filename + ".tsp.solution";
    ofstream file(dir);

    file << "NAME : " << dir << endl;
    file << "TYPE : TOUR" << endl;
    file << "DIMENSION : " << path.size() << endl;
    file << "TOUR_SECTION" << endl;

    for(int i = 0; i < path.size(); ++i) {
        file << path[i] << endl;
    }   
}

#endif