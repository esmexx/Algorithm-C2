#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
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


int main() {

	string clsfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w2_clustering_short.txt";
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




	
	return 0;
}