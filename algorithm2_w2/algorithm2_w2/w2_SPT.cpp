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
			if (stoi(tmp_ver) != (vc + 1)){ cout << "no!" << endl; };

			for (int i = 0; i < nc; i++){
				connection >> tmp_conn >> tmp_edge;
				ver_arr[vc][i] = HEAP_ELM(stoi(tmp_edge), stoi(tmp_conn));
			}
			vc++;
		}
	}

	// initialize distances of all vertices as infinite
	vector<int> dist; 
	dist.resize(NUM_VER);
	for (int i = 0; i < NUM_VER; i++){
		dist[i] = numeric_limits<int>::max();
	}

	priority_queue<HEAP_ELM, vector<HEAP_ELM>, HEAP_COMP> pq;
	// insert source vertex into pq and make its distance 0
	pq.push(HEAP_ELM(0, 0)); dist[0] = 0;


	while (!pq.empty()){
		int u = pq.top().getVer(); // extract minimum distance vertex from pq
		pq.pop();
		for (int i = 0; i < num_conn[u]; i++){
			int v = ver_arr[u][i].getVer() - 1;
			int w = ver_arr[u][i].getEdge();
			if (dist[v] > dist[u] + w){
				dist[v] = dist[u] + w;
				pq.push(HEAP_ELM(dist[v], v));
			}
		}

	}

	cout << dist[6] << endl;
	cout << dist[36] << endl;
	cout << dist[58] << endl;
	cout << dist[81] << endl;
	cout << dist[98] << endl;
	cout << dist[114] << endl;
	cout << dist[132] << endl;
	cout << dist[164] << endl;
	cout << dist[187] << endl;
	cout << dist[196] << endl;

	return 0;
}