#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stdio.h>
#include <set>

using namespace std;

class HEAP_COMP{
public:
	bool operator() (const pair<int, int>& v1, const pair<int, int>& v2){
		return v1.second < v2.second;
	}
};

int main() {

	string mstfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w1_edges.txt";
	string line, nvtmp, netmp, htmp, ttmp, ctmp;
	int i = 0, Nv, Ne;
	int** AdjMat = NULL;


	// build an undirected graph using adjacency matrix while reading data
	ifstream myfile(mstfile);
	if (myfile.is_open()) {
		while (getline(myfile, line)){
			if (i == 0) {
				stringstream connection(line);
				connection >> nvtmp >> netmp;
				Nv = stoi(nvtmp);
				Ne = stoi(netmp);

				// initialize the adjacency matrix with max
				if (AdjMat == NULL) {
					AdjMat = new int*[Nv];
					for (int j = 0; j < Nv; j++){
						AdjMat[j] = new int[Nv];
						for (int k = 0; k < Nv; k++){
							AdjMat[j][k] = numeric_limits<int>::max();
						}
					}
				}

				i++;
			}
			else {
				stringstream connection(line);
				connection >> htmp >> ttmp >> ctmp;

				// fill in both directions
				AdjMat[stoi(htmp) - 1][stoi(ttmp) - 1] = stoi(ctmp);
				AdjMat[stoi(ttmp) - 1][stoi(htmp) - 1] = stoi(ctmp);
			}
		}
	}



	// http://www.cdn.geeksforgeeks.org/prims-algorithm-using-priority_queue-stl/
	// the code above has some errors, fixed below

	vector<int> key(Nv, numeric_limits<int>::max());
	vector<bool> inMST(Nv, false);

	multiset<pair<int, int>, HEAP_COMP> mst;
	mst.insert(make_pair(0, 0));  key[0] = 0;
	int cost = 0;


	while (!mst.empty()){
		int u = mst.begin()->first;
		if (!inMST[u]){
			inMST[u] = true;
			cost += key[u];
			mst.erase(mst.begin());
		}
		else {
			// delete repeated (vertice, weight) pair from the heap 
			// if the vertice has been found before, then its weight 
			// is already added to the total cost
			mst.erase(mst.begin());
			continue;
		}

		for (int v = 0; v < Nv; v++){
			if (AdjMat[u][v] < numeric_limits<int>::max()){
				if (inMST[v] == false && key[v] > AdjMat[u][v]){
					key[v] = AdjMat[u][v];
					mst.insert(make_pair(v, key[v]));
				}
			}
		}
		
	}


	cout << "cost of the MST: " << cost << endl;



	return 0;

}