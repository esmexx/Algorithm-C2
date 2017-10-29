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
	int EdgeHead = -1;
	vector<pair<int, int>> EdgeTails; 
	int VecLen = 500; // initialize number of tails from the head node, default 500, expand if exceed
	// note that the graph is fully connected (there are 500 head nodes and 124750 edges)
	int NumTails = 0; // count number of tails

public:
	HEAP_ELM() { }
	HEAP_ELM(int _EdgeHead){
		EdgeHead = _EdgeHead;
		EdgeTails.resize(VecLen, make_pair(-1, 0));
	}

	int GetHead() const { return EdgeHead; }
	int GetNumTails() const { return NumTails; }

	void AddTail(int _EdgeTail, int _EdgeCost){
		if (NumTails < VecLen){
			EdgeTails[NumTails] = make_pair(_EdgeTail, _EdgeCost);
			NumTails++;
		}
		else { // expand vector if not enough
			VecLen *= 2;
			EdgeTails.resize(VecLen, make_pair(-1, 0));
			AddTail(_EdgeTail, _EdgeCost);
		}
	}

	pair<int, int> GetTail(int i) {
		return EdgeTails[i]; 	// get the ith tail
	}

};


class HEAP_COMP{
public:
	bool operator() (const pair<int, int>& v1, const pair<int, int>& v2){
		if (abs(v1.second - v2.second) < std::numeric_limits<double>::epsilon()){
			return true; // break tie "arbitrarily"
		}
		else {
			return v1.second < v2.second;
		}
	}
};


int main() {

	string clsfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w2_clustering.txt";
	string line, ntmp, htmp, ttmp, ctmp;
	HEAP_ELM* num = NULL;
	int i = 0, N;


	// build an undirected graph while reading data
	ifstream myfile(clsfile);


	if (myfile.is_open()) {
		while (getline(myfile, line)){
			if (i == 0){
				stringstream connection(line);
				connection >> ntmp;
				N = stoi(ntmp);
				if (num == NULL) { num = new HEAP_ELM[N]; } 
				i++;
			}
			else {
				stringstream connection(line);
				connection >> htmp >> ttmp >> ctmp;

				// head -> tail
				if (num[stoi(htmp) - 1].GetHead() == -1){
					num[stoi(htmp) - 1] = HEAP_ELM(stoi(htmp));
				}
				num[stoi(htmp) - 1].AddTail(stoi(ttmp), stoi(ctmp));

				// tail -> head
				if (num[stoi(ttmp) - 1].GetHead() == -1){
					num[stoi(ttmp) - 1] = HEAP_ELM(stoi(ttmp));
				}
				num[stoi(ttmp) - 1].AddTail(stoi(htmp), stoi(ctmp));
				
				i++;
			}

		}
	}

	

	return 0;
}