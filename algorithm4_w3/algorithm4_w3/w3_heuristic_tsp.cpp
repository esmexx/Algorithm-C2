#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <set>
#include <time.h>
#include <math.h>
#include <algorithm>

using namespace std;

double GetDist(pair<double, double> src, pair<double, double> dest) {
    return sqrt(pow(src.first - dest.first, 2) + pow(src.second - dest.second, 2));
}

class TspNode {
private:
    long city_num;
    pair<double, double> city_loc;
    double dist2src = DBL_MAX; // store the distance between this node and the head node of the tour

public:

    TspNode() {};
    TspNode(long _city_num, double _locx, double _locy){
        city_num = _city_num;
        city_loc = make_pair(_locx, _locy);
    }

    long GetCityNum() const { return city_num; }
    pair<double, double> GetCityLoc() const { return city_loc; }
    double GetDist2Src() const { return dist2src; }

    void CalcDist2Src(TspNode& src){
        dist2src = GetDist(city_loc, src.GetCityLoc());
    }

};

class TspComp {
public:
    bool operator() (const TspNode& n1, const TspNode& n2) const {
        if (fabs(n1.GetDist2Src() - n2.GetDist2Src()) < 1.0e-12f) {
            return n1.GetCityNum() < n2.GetCityNum();
        }
        else {
            return n1.GetDist2Src() < n2.GetDist2Src();
        }
    }
};




int main() {

    string tspfile = "w3_nn.txt";
    string line, tnc, tn, tx, ty;
    long i = -1, ncities;

    TspNode* num = NULL;

    // load the txt file and store the city coordinates
    ifstream myfile(tspfile);
    if (myfile.is_open()){
        while (getline(myfile, line)){
            if (i == -1){
                stringstream connection(line);
                connection >> tnc;
                ncities = stol(tnc);
                if (!num) { num = new TspNode[ncities]; }
                i++;
            }
            else {
                stringstream connection(line);
                connection >> tn >> tx >> ty;
                num[i] = TspNode(stol(tn), stod(tx), stod(ty));
                i++;
            }
        }
    }



    // https://en.wikipedia.org/wiki/Nearest_neighbour_algorithm
    // using heap to find the cloest city
    multiset<TspNode, TspComp> dist_heap;

    // keep track of visited cities
    bool* visited = new bool[ncities];
    for (long i = 0; i < ncities; i++){
        visited[i] = false; 
    }

    // start tour from the first city
    visited[0] = true; 
    TspNode tour_head = num[0];
    long num_visited_city = 1;

    // store the cost of the traveling salesman tour
    double tsp_cost = 0;

    // find out the shortest edge connecting the tour head and an unvisited node V
    while (num_visited_city < ncities){ // if all the nodes are visited, then terminate
        dist_heap.clear();
        for (long i = 0; i < ncities; i++){
            if (!visited[i]) {
                num[i].CalcDist2Src(tour_head);
                dist_heap.insert(num[i]);
            }
        }
        // set tour head to V
        tour_head = *dist_heap.begin();
        // mark V as visited;
        visited[tour_head.GetCityNum() - 1] = true;
        // add edge cost to total
        tsp_cost += tour_head.GetDist2Src();
        num_visited_city++;
        if (num_visited_city % 100 == 0) {
            cout << num_visited_city << endl;
            time = clock() - time;
            cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;
        }

    }

    // once every city has been visited exactly once, return to the first city to complete the tour
    tour_head.CalcDist2Src(num[0]);
    tsp_cost += tour_head.GetDist2Src();

    cout << "cost of traveling salesman tour is " << tsp_cost << endl;

    delete[] num;
    delete[] visited;

    return 0;
}