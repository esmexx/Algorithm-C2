#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <stdio.h>
#include <set>

using namespace std;


class HEAP_ELM {
	int EdgeHead = -1;
	vector<pair<int, int>> EdgeTails;  // http://www.geeksforgeeks.org/sorting-vector-of-pairs-in-c-set-1-sort-by-first-and-second/
	int NumTails = 0; // number of tails

public:
	HEAP_ELM() {};
	HEAP_ELM(int _EdgeHead){
		EdgeHead = _EdgeHead;
	}

	int GetHead() const { return EdgeHead; }
	int GetNumTails() const { return NumTails; }

	void AddTail(int _EdgeTail, int _EdgeCost){
		EdgeTails.push_back(make_pair(_EdgeTail, _EdgeCost));
		NumTails++;
	}

	pair<int, int> GetTail(int i) {
		return EdgeTails[i]; 	// get the ith tail
	}

};


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
	HEAP_ELM* num = NULL;


	// build an undirected graph while reading data
	ifstream myfile(mstfile);
	if (myfile.is_open()) {
		while (getline(myfile, line)){
			if (i == 0) {
				stringstream connection(line);
				connection >> nvtmp >> netmp;
				Nv = stoi(nvtmp); 
				Ne = stoi(netmp);
				if (num == NULL) { num = new HEAP_ELM[Nv]; } // not sure how to initialize the heap
				i++;
			}
			else {
				stringstream connection(line);
				connection >> htmp >> ttmp >> ctmp;
				
				// head -> tail
				if (num[stoi(htmp) - 1].GetHead() == -1){
					num[stoi(htmp) - 1] = HEAP_ELM(stoi(htmp));
				}
				num[stoi(htmp)-1].AddTail(stoi(ttmp), stoi(ctmp));

				// tail -> head
				if (num[stoi(ttmp) - 1].GetHead() == -1){
					num[stoi(ttmp) - 1] = HEAP_ELM(stoi(ttmp));
				}
				num[stoi(ttmp) - 1].AddTail(stoi(htmp), stoi(ctmp));

				i++;
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


		for (int i = 0; i < num[u].GetNumTails(); i++){
			int v = num[u].GetTail(i).first - 1;

			if (inMST[v] == false && key[v] > num[u].GetTail(i).second){
				key[v] = num[u].GetTail(i).second;
				mst.insert(make_pair(v, key[v]));
			}
		}
	}


	cout << "cost of the MST: " << cost << endl;


	return 0;

}