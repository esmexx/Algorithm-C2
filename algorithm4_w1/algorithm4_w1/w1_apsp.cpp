#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <algorithm>

using namespace std;


class GraphElm {
	int EdgeHead = -1;
	vector<pair<int, int>> EdgeTails;
	int NumTails = 0;

public:
	GraphElm() {};
	GraphElm(int _EdgeHead){
		EdgeHead = _EdgeHead;
	}

	int GetHead() const { return EdgeHead; }
	int GetNumTails() const { return NumTails; }

	void AddTail(int _EdgeTail, int _EdgeCost) {
		EdgeTails.push_back(make_pair(_EdgeTail, _EdgeCost));
		NumTails++;
	}

	pair<int, int> GetTail(int i){
		return EdgeTails[i];
	}
};


int BellmanFord(GraphElm* graph, int num_ver, int num_edge, int src){

	int shortest_dist;

	// create an array dist[] of size |V| with all values as infinite except dist[src]
	int* dist = new int[num_ver];
	for (int i = 0; i < num_ver; i++){
		dist[i] = INT_MAX;
	}
	dist[src - 1] = 0;

	for (int i = 1; i < num_ver; i++){
		bool stop = true; // stop early if dist doesn't change
		for (int j = 0; j < num_ver; j++) { // loop through all edges and update
			for (int k = 0; k < graph[j].GetNumTails(); k++) {
				int u = graph[j].GetHead() - 1;
				int v = graph[j].GetTail(k).first - 1;
				int cost = graph[j].GetTail(k).second;
				if (dist[u] != INT_MAX && dist[u] + cost < dist[v]) {
					dist[v] = dist[u] + cost;
				    stop = false;
				}
			}
		}
        if (stop) break;
	}


	// detect negative cycle
	for (int j = 0; j < num_ver; j++) { // loop through all edges and update
		for (int k = 0; k < graph[j].GetNumTails(); k++) {
			int u = graph[j].GetHead() - 1;
			int v = graph[j].GetTail(k).first - 1;
			int cost = graph[j].GetTail(k).second;
			if (dist[u] != INT_MAX && dist[u] + cost < dist[v]){
				cout << "Graph contains negative cycle." << endl;
				delete[] dist;

				return INT_MIN;
			}
		}
	}

	shortest_dist = *min_element(dist, dist + num_ver);
	delete[] dist;

	return shortest_dist;

}

int main() {

	clock_t time;
	time = clock();

	string apfile = "w1_g3.txt";
	string line, vtmp, etmp, htmp, ttmp, ctmp;
	int i = -1, V, E;

	GraphElm* num = NULL;

	ifstream myfile(apfile);
	if (myfile.is_open()){
		while (getline(myfile, line)) {
			if (i == -1){
				stringstream connection(line);
				connection >> vtmp >> etmp;
				V = stoi(vtmp);
				E = stoi(etmp);
				if (num == NULL) { num = new GraphElm[V]; }
				i++;
			}
			else {
				stringstream connection(line);
				connection >> htmp >> ttmp >> ctmp;

				// head -> tail
				if (num[stoi(htmp) - 1].GetHead() == -1){
					num[stoi(htmp) - 1] = GraphElm(stoi(htmp));
				}
				num[stoi(htmp) - 1].AddTail(stoi(ttmp), stoi(ctmp));

				i++;

			}
		}
	}

	// http://www.geeksforgeeks.org/dynamic-programming-set-23-bellman-ford-algorithm/

	int shortest_shortest_path = INT_MAX;

	for (int i = 0; i < V; i++){
		int d = BellmanFord(num, V, E, i + 1);
		if (d < shortest_shortest_path)
			shortest_shortest_path = d;
	}

	cout << shortest_shortest_path << endl;

	time = clock() - time;
	cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;

	return 0;

}