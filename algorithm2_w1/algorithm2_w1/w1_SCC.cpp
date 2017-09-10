#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <algorithm>
#include <vector>
#include <stack>
#define NUM_VER 875714
#define NUM_EDGE 5105043
//#define NUM_VER 3200
//#define NUM_EDGE 4523
using namespace std;

/* ----------------------------------------------------------------------- */

bool exist_repeated_node(int node, vector<int> leader_vector){

	for (int i = 0; i < leader_vector.size(); i++){
		if (node == leader_vector[i])
			return true;
	}
	return false;
}

/* ---------------------------------Grev---------------------------------- */

void DFS_rv(int **map, int *conn_ct, int i, int &t, int *fin_time, bool *expl_node) {

	expl_node[i] = true;

	for (int k = 0; k < conn_ct[i]; k++){
		int j = map[i][k] - 1;
		if (!expl_node[j]){
			DFS_rv(map, conn_ct, j, t, fin_time, expl_node);
		}
	}
	t++;
	fin_time[i] = t;

}

// http://www.roman10.net/2011/05/21/depth-first-graph-traversal-using-stack-a-c-implementation/
// visit one node, and then pop one
// cannot be used to get finishing time
void DFSstack_rv(int **map, int *conn_ct, int i, int *fin_time, bool *expl_node){

	stack<int> S;
	S.push(i);

	while (!S.empty()){
		int v = S.top();
		S.pop();
		expl_node[v] = true;
		for (int k = 0; k < conn_ct[v]; k++){
			int w = map[v][k] - 1;
			if (!expl_node[w]){
				S.push(w);
			}
		}

	}

}

// implementation of obtaining finishing time with stack
void DFSstack_rv_ft(int **map, int *conn_ct, int i, int &t, int *fin_time, bool *expl_node){


	stack<int> S;
	S.push(i);
	expl_node[S.top()] = true;

	if (map[i][0] == -2){
		t++;
		fin_time[i] = t;
		S.pop();
	}

	while (!S.empty()){

		int v = S.top();
		int k;

		for (k = 0; k < conn_ct[v]; k++){
			int w = map[v][k] - 1;
			if (!expl_node[w]){
				S.push(w);
				expl_node[w] = true;
				break;
			}
		}

		if (k == conn_ct[v]){
			t++;
			fin_time[S.top()] = t;
			S.pop();
		}

	}

}



void DFSloop_rv(int **map, int *conn_ct, int *fin_time){

	int t = 0;

	bool *expl_node = new bool[NUM_VER]; // whether the node has been explored
	for (int i = 0; i < NUM_VER; i++)
		expl_node[i] = false;


	for (int i = NUM_VER - 1; i >= 0; i--){
		if (!expl_node[i]){
			// DFS_rv(map, conn_ct, i, t, fin_time, expl_node);
			DFSstack_rv_ft(map, conn_ct, i, t, fin_time, expl_node);
		}
	}

	delete[] expl_node;
}



/* -------------------------------Graph----------------------------------- */


void DFS(int **map, int *conn_ct, int i, int s, bool *expl_node, int *leader_node){

	expl_node[i] = true;
	leader_node[i] = s;

	for (int k = 0; k < conn_ct[i]; k++){
		int j = map[i][k] - 1;
		if (!expl_node[j]){
			DFS(map, conn_ct, j, s, expl_node, leader_node);
		}
	}

}


void DFSstack_ft(int **map, int *conn_ct, int i, int s, bool *expl_node, int *leader_node){

	stack<int> S;
	S.push(i);
	expl_node[S.top()] = true;
	leader_node[S.top()] = s;

	if (map[i][0] == -2){
		S.pop();
	}

	while (!S.empty()){

		int v = S.top();
		int k;

		for (k = 0; k < conn_ct[v]; k++){
			int w = map[v][k] - 1;
			if (!expl_node[w]){
				S.push(w);
				expl_node[w] = true;
				leader_node[S.top()] = s;
				break;
			}
		}

		if (k == conn_ct[v]){
			S.pop();
		}

	}

}


void DFSloop(int **map, int *conn_ct, int *fin2ver, int *leader_node){

	int s = -1;

	bool *expl_node = new bool[NUM_VER]; // whether the node has been explored
	for (int i = 0; i < NUM_VER; i++)
		expl_node[i] = false;

	for (int i = NUM_VER - 1; i >= 0; i--){
		int n = fin2ver[i];
		if (n > 0){
			if (!expl_node[n]){
				s = n + 1;
				// DFS(map, conn_ct, n, s, expl_node, leader_node);
				DFSstack_ft(map, conn_ct, n, s, expl_node, leader_node);
			}
		}
		//else {
		//	leader_node[i] = -1;
		//}
	}

	delete[] expl_node;
}


/* ----------------------------------------------------------------------- */


int main(){

	clock_t time;
	string line, vert, verh;
	int ver_tail, ver_head;

	int *ver_tails = new int[NUM_EDGE];
	int *ver_heads = new int[NUM_EDGE];


	// build the graph
	//int *ver_arr = new int[NUM_VER]; // store the vertice; resolve stack overflow (why can't int ver_arr[NUM_VER]?)
	int *conn_count = new int[NUM_VER]; // count # of conns for each vertice
	int *conn_count_c = new int[NUM_VER];
	//vector<int> conn_vers; // store the connections for each vertice

	// build the reversed graph
	int *conn_count_rv = new int[NUM_VER]; // count # of conns for each vertice in reversed graph
	int *ver_arr_rv = new int[NUM_VER]; // store the corresponding vertice
	int *conn_count_rv_c = new int[NUM_VER]; // for filling in the connections

	for (int i = 0; i < NUM_VER; i++) {
		//ver_arr[i] = -1;
		ver_arr_rv[i] = -1;
		conn_count[i] = 0;
		conn_count_c[i] = 0;
		conn_count_rv[i] = 0;
		conn_count_rv_c[i] = 0;
	}


	//string sccfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 2\\SCC_short.txt";
	string sccfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 2\\SCC.txt";
	ifstream myfile(sccfile);

	time = clock();

	int past_ver_tail = 1;
	int edge_count = 0;

	if (myfile.is_open()){
		while (getline(myfile, line)){
			if (!line.empty()){
				// prase line as inputs
				stringstream connection(line);
				connection >> vert >> verh;
				// convert string to integer
				ver_tail = stoi(vert);
				ver_head = stoi(verh);

				ver_tails[edge_count] = ver_tail;
				ver_heads[edge_count] = ver_head;
				edge_count++;

				conn_count[ver_tail - 1]++;
				if (ver_tail != past_ver_tail){
					past_ver_tail = ver_tail;
					//ver_arr[ver_tail - 2] = ver_tail;
				}

				//if (ver_tail != past_ver_tail){
				//	past_ver_tail = ver_tail;
				//	ver_map[ver_tail - 2] = new int[conn_vers.size()];
				//	for (int i = 0; i < conn_vers.size(); i++){
				//		ver_map[ver_tail - 2][i] = conn_vers[i]; // transfer the connections in vector to the 2d map
				//	}
				//	conn_count[ver_tail - 2] = conn_vers.size();
				//	conn_vers.clear();
				//}
				//conn_vers.push_back(ver_head); // use conn_vers to store the connections to each vertice

				// record # of conns for each vertice in the reversed graph
				// ver_arr_rv[0]=1, ver_arr_rv[1]=2, ...
				conn_count_rv[ver_head - 1]++;
				ver_arr_rv[ver_head - 1] = ver_head;
			}
		}
		myfile.close();
	}
	// take care of the last element



	int **ver_map = new int*[NUM_VER]; // 2d array storing graph
	for (int i = 0; i < NUM_VER; i++){
		if (conn_count[i]>0){
			ver_map[i] = new int[conn_count[i]];
			for (int j = 0; j < conn_count[i]; j++)
				ver_map[i][j] = 0;
		}
		else {
			ver_map[i] = new int[1];
			ver_map[i][0] = -2;
		}
	}

	// insertion
	for (int i = 0; i < NUM_EDGE; i++){
		int curr_ver_tail = ver_tails[i] - 1;
		if (conn_count[curr_ver_tail]>0){
			if (conn_count_c[curr_ver_tail] < conn_count[curr_ver_tail]){
				ver_map[curr_ver_tail][conn_count_c[curr_ver_tail]] = ver_heads[i];
				conn_count_c[curr_ver_tail]++;
			}
		}
	}


	// setup 2d array for the reversed graph
	// if the vertice has at least 1 connection, open up an array and put -1's as placeholder
	// if the vertice has no connection, it would have only one element which is -2
	int **ver_map_rv = new int*[NUM_VER];
	for (int i = 0; i < NUM_VER; i++){
		if (conn_count_rv[i]>0){
			ver_map_rv[i] = new int[conn_count_rv[i]];
		}
		else {
			ver_map_rv[i] = new int[1];
			ver_map_rv[i][0] = -2;
		}
	}


	// fill in the 2d array for the reversed graph
	for (int i = 0; i < NUM_EDGE; i++){
		if ((ver_map_rv[ver_heads[i] - 1][0] != -2) && (conn_count_rv_c[ver_heads[i] - 1] < conn_count_rv[ver_heads[i] - 1])){
			ver_map_rv[ver_heads[i] - 1][conn_count_rv_c[ver_heads[i] - 1]] = ver_tails[i];
			conn_count_rv_c[ver_heads[i] - 1]++;
		}
	}

	delete[] ver_tails;
	delete[] ver_heads;

	std::cout << "done building the two graphs" << endl;
	// Run DFS-Loop on Grev

	int *fin_time = new int[NUM_VER];
	int *leader_node = new int[NUM_VER];

	for (int i = 0; i < NUM_VER; i++){
		fin_time[i] = -1;
		leader_node[i] = -1;
	}

	DFSloop_rv(ver_map_rv, conn_count_rv, fin_time);
	std::cout << "done DFSloop on the reversed graph" << endl;

	int *fin2ver = new int[NUM_VER]; // if fin_time[1]=6, then fin2ver[5]=1
	for (int i = 0; i < NUM_VER; i++){
		fin2ver[fin_time[i] - 1] = i;
	}

	DFSloop(ver_map, conn_count, fin2ver, leader_node);
	std::cout << "done DFSloop on the graph and find leading node" << endl;


	// very time-consuming codes below
	// get the unique leader nodes
	//vector<int> unique_ln; // unique leader nodes
	//unique_ln.push_back(leader_node[0]);
	//for (int i = 1; i < NUM_VER; i++){
	//	if (!exist_repeated_node(leader_node[i], unique_ln) && leader_node[i] != -1){
	//		unique_ln.push_back(leader_node[i]);
	//	}
	//}

	int *unique_leaders = new int[NUM_VER];
	for (int i = 0; i < NUM_VER; i++)
		unique_leaders[i] = 0;

	unique_leaders[0] = leader_node[0];
	int leaders_count = 1;
	for (int i = 1; i < NUM_VER; i++){
		bool exists = find(unique_leaders, unique_leaders + leaders_count, leader_node[i]) != (unique_leaders + leaders_count);
		if (!exists && leader_node[i] != -1){
			unique_leaders[leaders_count] = leader_node[i];
			leaders_count++;
		}
	}

	//int *unique_leaders = new int[NUM_VER];
	//for (int i = 0; i < NUM_VER; i++)
	//	unique_leaders[i] = 0;
	//
	//unique_leaders[0] = leader_node[0];
	//int leaders_count = 1;
	//for (int i = 0; i < NUM_VER; i++){
	//	if (leader_node[i] != -1){
	//		bool exists = find(unique_leaders, unique_leaders + leaders_count, leader_node[i]) != (unique_leaders + leaders_count);
	//		if (exists){
	//			unique_leaders[leaders_count - 1] = leader_node[i];
	//			leaders_count++;
	//		}
	//	}
	//}
	//

	std::cout << "done getting unique leader nodes" << endl;

	//int *unique_ln_count = new int[unique_ln.size()];
	int *unique_ln_count = new int[leaders_count];
	//for (int i = 0; i < unique_ln.size(); i++){
	for (int i = 0; i < leaders_count; i++){
		unique_ln_count[i] = count(leader_node, leader_node + NUM_VER, unique_leaders[i]);
	}

	std::cout << "done building leader node histogram" << endl;
	//unique_ln.resize(NUM_VER);
	//for (int i = 0; i < NUM_VER; i++){
	//	unique_ln[i] = leader_node[i];
	//}
	//sort(unique_ln.begin(), unique_ln.end());
	//vector<int>::iterator it = unique(unique_ln.begin(), unique_ln.end());
	//unique_ln.resize(distance(unique_ln.begin(),it));



	// count the repeats of each unique leader nodes
	//int *unique_ln_count = new int[unique_ln.size()];
	//for (int i = 0; i < unique_ln.size(); i++)
	//	unique_ln_count[i] = 0;

	//for (int i = 0; i < NUM_VER; i++){
	//	for (int j = 0; j < unique_ln.size(); j++){
	//		if (leader_node[i] == unique_ln[j])
	//			unique_ln_count[j]++;
	//	}

	//}

	std::sort(unique_ln_count, unique_ln_count + leaders_count);

	for (int i = 0; i < leaders_count; i++)
		std::cout << unique_ln_count[i] << " ";
	std::cout << endl;

	time = clock() - time;
	std::cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;


	// delete existing arrays
	//delete[] ver_arr;
	delete[] ver_arr_rv;
	delete[] conn_count;
	delete[] conn_count_c;
	delete[] conn_count_rv;
	delete[] conn_count_rv_c;
	delete[] fin2ver;
	delete[] unique_ln_count;
	delete[] fin_time;
	delete[] leader_node;
	for (int i = 0; i < NUM_VER; i++){
		delete[] ver_map[i];
	}
	delete[] ver_map;

	for (int i = 0; i < NUM_VER; i++){
		delete[] ver_map_rv[i];
	}
	delete[] ver_map_rv;
	return 0;
}