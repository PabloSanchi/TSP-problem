#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <numeric>
#include <cmath>

#define N 1E10
#define TMIN 1E-14
#define MAX 2E5

using namespace std;

vector<int> randperm(size_t n) {
    vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(v.begin(), v.end(), rng);

    return v;
}

vector<int> getSuccessor(const vector<int>& state) {
    vector<int> newS(state);

    size_t first = rand() % state.size();
    size_t second = rand() % state.size();
    if(first > second) swap(first, second);

    flip(newS, first, second);
    
    return newS;
}

void flip(vector<int>& state, size_t a, size_t b) {
    while(a < b) {
        swap(state[a], state[b]);
        a++;
        b--;
    }
}

size_t cooldown(size_t t, size_t i) {
    size_t r = t;
    if(i%100 == 0) r = t * 0.9;
    return r;
}

size_t eval(const vector<int>& state, const vector<vector<int>>& costs) {
    size_t cost = 0;
    for(int i = 0; costs.size()-1; ++i) {
        cost += costs[state[i]][state[i+1]];
    }
    cost += costs[state[state.size()-1]][state[0]];
    return cost;
}

pair<vector<int>, size_t> annealing(const vector<vector<int>>& nodes, size_t n, size_t t, size_t tMin, size_t max) {
    vector<int> current = randperm(n);
    
    size_t cost = 0;
    size_t cooldownCounter = 0;
    size_t iterCounter = 1;

    while( t > tMin && iterCounter < max) {
        // get successor
        vector<int> newS = getSuccessor(current);
        // get the delta, eval(successor) - eval(current)
        size_t deltaE = eval(newS, nodes) - eval(current, nodes);
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
    }

    return {current, cost};
}

int main(void) {

    

    return 0;
}