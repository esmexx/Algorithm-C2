#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#define NUM_VER 200

using namespace std;

// http://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/

// heap element {edge weight, vertice}
class HEAP_ELM {
	int Edge;
	int Ver;
public:
	HEAP_ELM() {};
	HEAP_ELM(int _Edge, int _Ver){
		Edge = _Edge;
		Ver = _Ver;
	}
	int getEdge() const { return Edge; }
	int getVer() const { return Ver; }
};

// comparator for building min heap
class HEAP_COMP{
public: 
	bool operator() (const HEAP_ELM& v1, HEAP_ELM& v2){
		return v1.getEdge() > v2.getEdge();
	}
};

int main(){

	HEAP_ELM** ver_arr = new HEAP_ELM*[NUM_VER]; // 2d map storing vertices and their connections
	int* num_conn = new int[NUM_VER]; // 1d array storing number of connections for each vertice
	int vc = 0; // vertice counter

	string sptfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 2\\\\dijkstraData.txt";
	string line, tmp_ver, tmp_conn, tmp_edge;

	ifstream myfile(sptfile);
	if (myfile.is_open()){
		while (getline(myfile, line)){
			int nc = count(line.begin(), line.end(), '\t') - 1; // get number of connected vertices by counting # of '\t' in the string
			
			num_conn[vc] = nc; 
			ver_arr[vc] = new HEAP_ELM[nc]; // for each vertice, create an array storing connection and edge weight

			replace_if(line.begin(), line.end(), ispunct, ' ');  // replace "," with space
			stringstream connection(line);
			connection >> tmp_ver;
			if (stoi(tmp_ver) != (vc + 1)){ cout << "no!" << endl; }; // check if the vertices are in order

			for (int i = 0; i < nc; i++){
				connection >> tmp_conn >> tmp_edge;
				ver_arr[vc][i] = HEAP_ELM(stoi(tmp_edge), stoi(tmp_conn));
			}
			vc++;
		}
	}

	// initialize distances of all vertices as infinite
	vector<int> A; 
	A.resize(NUM_VER);
	for (int i = 0; i < NUM_VER; i++){
		A[i] = numeric_limits<int>::max();
	}

	priority_queue<HEAP_ELM, vector<HEAP_ELM>, HEAP_COMP> spt;
	// insert source vertex into spt and make its distance 0
	spt.push(HEAP_ELM(0, 0)); A[0] = 0;


	while (!spt.empty()){
		int v = spt.top().getVer(); 
		spt.pop();

		for (int i = 0; i < num_conn[v]; i++){
			int w = ver_arr[v][i].getVer() - 1;
			int l_vw = ver_arr[v][i].getEdge();
			// pick only the minimum (A[v] + l_vw)
			if (A[w] > A[v] + l_vw){
				A[w] = A[v] + l_vw;
				spt.push(HEAP_ELM(A[w], w));
			}
		}

	}

	cout << A[6] << endl;
	cout << A[36] << endl;
	cout << A[58] << endl;
	cout << A[81] << endl;
	cout << A[98] << endl;
	cout << A[114] << endl;
	cout << A[132] << endl;
	cout << A[164] << endl;
	cout << A[187] << endl;
	cout << A[196] << endl;


	return 0;
}