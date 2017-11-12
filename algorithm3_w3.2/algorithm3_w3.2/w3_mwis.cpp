#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <algorithm>

using namespace std;

int main() {

	string mwisfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w3_mwis.txt";
	string line, ntmp, wtmp;
	int ct = -1, num_nodes;
	long long* node_arr = NULL;
	bool* node_mwis = NULL;

	ifstream myfile(mwisfile);
	if (myfile.is_open()){
		while (getline(myfile, line)) {
			if (ct == -1) {
				stringstream connection(line);
				connection >> ntmp;
				num_nodes = stoi(ntmp);
				if (!node_arr) {
					node_arr = new long long[num_nodes];
					node_mwis = new bool[num_nodes];
				}
				ct++;
			}
			else {
				stringstream connection(line);
				connection >> wtmp;
				node_arr[ct] = stoll(wtmp);
				node_mwis[ct] = false;
				ct++;
			}
		}
	}

	// Let A be the filled-in array
	// in lecture slides, A[0]=0, A[1]=w1, but the vertex index actually starts from 1, and A[0] is merely 
	// for the case when i=2 and i-2=0
	long long* A = new long long[num_nodes];
	A[0] = node_arr[0];
	A[1] = node_arr[0] > node_arr[1]? node_arr[0]:node_arr[1];

	for (int i = 2; i < num_nodes; i++){
		A[i] = max(A[i - 1], A[i - 2] + node_arr[i]);
	}

	// reconstruction algorithm to get max-weight IS
	int k = num_nodes - 1;
	long long S = 0;

	while (k > 1) {
		if (A[k - 1] >= (A[k - 2] + node_arr[k])) {
			k--;
		}
		else {
			node_mwis[k] = true;
			S += node_arr[k];
			k = k - 2;
		}
	}

	// properly handle base cases
	if (node_mwis[3]){
		A[1] > A[0] ? node_mwis[1] = true : node_mwis[0] = true;

		if (node_mwis[1]) S += node_arr[1];
		else S += node_arr[0];
	}

	if (node_mwis[2]){
		node_mwis[0] = true;
		S += node_arr[0];
	}
	

	int node_in[8] = { 1, 2, 3, 4, 17, 117, 517, 997}; // no vertex 0
	int node_inb[8] = { 0, 0, 0, 0, 0, 0, 0, 0};

	for (int i = 0; i < 8; i++){
		if (node_mwis[node_in[i]-1]){
			node_inb[i] = 1;
		}
	}


	cout << "max sum: " << S << endl;

	for (int i = 0; i < 8; i++){
		cout << node_inb[i] << " ";
	}
	cout << endl;
	

	delete[] node_arr;
	delete[] node_mwis;
	delete[] A;
	return 0;
}