#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#define NUM_VER 200

using namespace std;

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
			if (stoi(tmp_ver) != (vc + 1)){ cout << "no!" << endl; };

			for (int i = 0; i < nc; i++){
				connection >> tmp_conn >> tmp_edge;
				ver_arr[vc][i] = HEAP_ELM(stoi(tmp_edge), stoi(tmp_conn));
			}
			vc++;
		}
	}


	//int *dist = new int[NUM_VER];
	//dist[0] = 0;
	//for (int i = 1; i < NUM_VER; i++)
	//	dist[i] = numeric_limits<int>::max();

	HEAP_ELM* output = new HEAP_ELM[NUM_VER];
	output[0] = HEAP_ELM(0,0);
	for (int i = 0; i < NUM_VER; i++){
		output[i] = HEAP_ELM(numeric_limits<int>::max(), i);
	}


	priority_queue<HEAP_ELM, vector<HEAP_ELM>, HEAP_COMP> spt_to_be;
	spt_to_be.push(HEAP_ELM(0, 0)); // assign 0 weight for the source vertex
	//for (int i = 1; i < NUM_VER; i++){
	//	spt_to_be.push(HEAP_ELM(numeric_limits<int>::max(), i)); // assign INF for all other vertices
	//}

	while (!spt_to_be.empty()){
		//HEAP_ELM htop = spt_to_be.top();
		//output[htop.getVer()] = htop;

		//int add_w = htop.getEdge();
		//
		//while (!spt_to_be.empty()){
		//	spt_to_be.pop();
		//}

		//for (int i = 0; i < num_conn[htop.getVer()]; i++){
		//	HEAP_ELM ver_conn = ver_arr[htop.getVer()][i];
		//	spt_to_be.push(HEAP_ELM(ver_conn.getEdge() + add_w, ver_conn.getVer()));
		//}

	}


	//while (!spt_to_be.empty()){
	//	HEAP_ELM htop = spt_to_be.top();
	//	for (int i = 0; i < num_conn[htop.getVer()]; i++){
	//		int u = ver_arr[htop.getVer()][i].getVer();
	//		int w = ver_arr[htop.getVer()][i].getEdge();
	//		
	//	}
	//}


	//HEAP_ELM p = pq.top();
	//cout << p.getEdge() << " " << p.getVer() << endl;
	//pq.pop();
	//p = pq.top();


	return 0;
}