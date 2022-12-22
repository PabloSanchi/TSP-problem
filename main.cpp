#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <numeric>
#include <cmath>
#include "readFile.hpp"

#define T 1E15
#define TMIN 1E-14
#define MAX 4E6

using namespace std;

vector<int> randperm(size_t n) {
    vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(v.begin(), v.end(), rng);

    return v;
}

void flip(vector<int>& state, size_t a, size_t b) {
    while(a < b) {
        swap(state[a], state[b]);
        a++;
        b--;
    }
}

vector<int> getSuccessor(const vector<int>& state) {
    vector<int> newS(state);

    size_t first = rand() % state.size();
    size_t second = rand() % state.size();
    if(first > second) swap(first, second);

    flip(newS, first, second);
    
    return newS;
}

double cooldown(double t, size_t i) {
    double r = t;
    if(i%100== 0) r = t * 0.99;
    return r;
}

double eval(vector<int>& state, const vector<vector<int>>& costs) {
    double cost = 0;
    
    for(int i = 0; i < state.size()-1; ++i)
        cost += costs[state[i]][state[i+1]];
    
    cost += costs[state[state.size()-1]][state[0]];

    return cost;
}

pair<vector<int>, size_t> annealing(const vector<vector<int>>& nodes, size_t n, double t, double tMin, size_t max) {
    vector<int> current = randperm(n);
    
    size_t cost = 0;
    size_t cooldownCounter = 0;
    size_t iterCounter = 1;

    while(t > tMin && iterCounter < max) {
        // get successor
        vector<int> newS = getSuccessor(current);
        // get the delta, eval(successor) - eval(current)
        double evalA = eval(newS, nodes);
        double evalB = eval(current, nodes);
        double deltaE = evalA - evalB;
        // if the successor is better than the current then current = successor
        if(deltaE <= 0) {
            current = newS;
            t = cooldown(t, cooldownCounter);
            cooldownCounter++;
        } else {
            double p = exp(-deltaE/t);
            if(p > (double)rand() / RAND_MAX)
                current = newS;
        }

        iterCounter++;

        // if(iterCounter%1000) {
        //     cout << iterCounter << endl;
        // }
    }
    cost = eval(current, nodes);
    return {current, cost};
}

int main(void) {

    // ask user for a filename
    string filename;
    cout << "Enter a filename: ";
    getline(std::cin, filename);

    cout << "Starting..." << endl;

    vector<pair<int,int>> cities = readFile("./TSPLIB/" + filename + ".tsp");
    vector<vector<int>> nodes = getDistances(cities);
    vector<int> optimal = getNodes("./TSPLIB/" + filename + ".opt.tour");
    
    auto start = chrono::high_resolution_clock::now();    
    auto [sol, cost] = annealing(nodes, nodes.size(), T, TMIN, MAX);
    auto end = chrono::high_resolution_clock::now();

    cout << "Cost: " << cost << endl;
    cout << "Best Cost: " << eval(optimal, nodes) << endl;

    writeToFile(sol, filename);

    cout << "Finished!" << endl;

    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    cout << "Time: " << duration.count() << " seconds" << endl;

    return 0;
}