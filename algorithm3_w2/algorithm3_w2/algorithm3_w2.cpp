#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <set>

#define NUM_CLS 4
using namespace std;

class HEAP_ELM {
	int head;
	int tail;
	int cost;

public:
	HEAP_ELM() {}
	HEAP_ELM(int _head, int _tail, int _cost) {
		head = _head;
		tail = _tail;
		cost = _cost;
	}

	int GetHead() const { return head; }
	int GetTail() const { return tail; }
	int GetCost() const { return cost; }

};


class HEAP_COMP{
public:
	bool operator() (const HEAP_ELM& v1, const HEAP_ELM& v2){
		return v1.GetCost() < v2.GetCost();   // multiset handles cost tie arbitrarily
	}
};



int Find(int parent[], int i){
	if (parent[i] == -1){
		return i;
	}
	else {
		return Find(parent, parent[i]);
	}
}


void Union(int parent[], int i, int j){
	int pi = Find(parent, i);
	int pj = Find(parent, j);

	parent[pi] = pj; 
}

bool doContinue(int parent[], int N){
	// return true until only NUM_CLS clusters
	int num_cls = 0;
	for (int i = 0; i < N; i++){
		if (parent[i] == -1)
			num_cls++;
	}
	return num_cls > NUM_CLS;
}

bool isCycle(int parent[], int i, int j){
	int pi = Find(parent, i);
	int pj = Find(parent, j);

	return (abs(pi - pj) < std::numeric_limits<double>::epsilon());
}


int main() {

	string clsfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w2_clustering.txt";
	string line, ntmp, htmp, ttmp, ctmp;
	int M = -1, N;
	multiset<HEAP_ELM, HEAP_COMP> kcluster;


	// build an undirected graph while reading data
	ifstream myfile(clsfile);
	if (myfile.is_open()) {
		while (getline(myfile, line)){
			if (M == -1){
				stringstream connection(line);
				connection >> ntmp;
				N = stoi(ntmp);
				M++;
			}
			else {
				stringstream connection(line);
				connection >> htmp >> ttmp >> ctmp;
				kcluster.insert(HEAP_ELM(stoi(htmp), stoi(ttmp), stoi(ctmp)));
				M++;
			}
		}
	}

	// initialize parent array
	int* parent = new int[N];
	for (int i = 0; i < N; i++){
		parent[i] = -1;
	}

	
	while (!kcluster.empty()) {
		if (doContinue(parent, N)){
			int h = kcluster.begin()->GetHead();
			int t = kcluster.begin()->GetTail();

			kcluster.erase(kcluster.begin());
			if (isCycle(parent, h - 1, t - 1)) {
				continue;
			}
			else {
				Union(parent, h - 1, t - 1);
			}

		}
		else {
			int h = kcluster.begin()->GetHead();
			int t = kcluster.begin()->GetTail();

			if (isCycle(parent, h - 1, t - 1)) {
				kcluster.erase(kcluster.begin());
			}
			else {
				cout << "max spacing: " << kcluster.begin()->GetCost() << endl;
				break;
			}
		}

	}

	
	return 0;
}