#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stdio.h>
#include <set>

using namespace std;

class HEAP_ELM {
	int EdgeHead;
	int EdgeTail;
	int EdgeCost;

public:
	HEAP_ELM() {};
	HEAP_ELM(int _EdgeHead, int _EdgeTail, int _EdgeCost){
		EdgeHead = _EdgeHead;
		EdgeTail = _EdgeTail;
		EdgeCost = _EdgeCost;
	}

	int GetHead() const { return EdgeHead; }
	int GetTail() const { return EdgeTail; }

	int GetCost() const { return EdgeCost; }
	int SetCost(int val) { EdgeCost = val; }


};

class HEAP_COMP {
public:
	bool operator() (const HEAP_ELM& v1, const HEAP_ELM& v2) {
		return v1.GetCost() < v2.GetCost(); // not sure whether this is the criteria for building the heap
	}
};

int main() {

	string mstfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w1_edges.txt";
	string line, nvtmp, netmp, htmp, ttmp, ctmp;
	int i = 0, Nv, Ne;
	HEAP_ELM* num = NULL;

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

			}

		}

	}


	return 0;

}