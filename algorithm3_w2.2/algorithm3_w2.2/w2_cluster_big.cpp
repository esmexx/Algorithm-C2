#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <unordered_set>


using namespace std;

//long Find(long parent[], long i){
//	if (parent[i] == 235779){
//		cout << "ERROR" << endl;
//	}
//
//	if (parent[i] == -1) {
//		return i;
//	}
//	else{
//		return Find(parent, parent[i]);
//	}
//}

long Find(long parent[], long i){
	long pi = i;
	while (parent[pi] != -1){
		pi = parent[pi];
	}
	return pi;
}



void Union(long parent[], long i, long j){
	long pi = Find(parent, i);
	long pj = Find(parent, j);

	parent[pi] = pj;
}

bool isCycle(long parent[], long i, long j){
	long pi = Find(parent, i);
	long pj = Find(parent, j);

	return pi == pj;
}


int main() {

	clock_t time;
	time = clock();

	string clsfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w2_clustering_big.txt";
	
	string line, Ne, Nb, Te, TeStr;

	unordered_set<string> NodeSet;

	long num_cls = -1;
	int num_bit = 0;
	ifstream myfile(clsfile);

	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if (num_cls == -1){
				stringstream connection(line);
				connection >> Ne >> Nb;
				num_cls = stoi(Ne); // number of nodes
				num_bit = stoi(Nb); // number of bits defining each node
			}
			else {
				stringstream connection(line);
				TeStr = "";
				for (int i = 0; i < num_bit; i++){
					connection >> Te;
					TeStr += Te;
				}
				NodeSet.insert(TeStr); // insert string to the hash table 
			}
		}
	}

	num_cls = NodeSet.size(); // count unique nodes (since repeated nodes were ignored by unordered_set)
	long num_parent = (long) pow(2, num_bit);
	long* parent = new long[num_parent]; // to keep track of parents, convert binary node to decimal and assign leading node

	for (long i = 0; i < num_parent; i++){
		parent[i] = -2;
	}


	// consider each node as an individual cluster
	unordered_set<string>::iterator iter = NodeSet.begin();
	for (long i = 0; i < num_cls; i++){
		string be = *iter;

		long Ndec=0;
		for (int j = 0; j < num_bit; j++){
			Ndec += (be[j] - '0') * pow(2, num_bit - j - 1); // convert string binary to decimal 
		}

		parent[Ndec] = -1; // assign self-leading node to every presented node

		iter++;
	}


	// union all the node pairs with distance 1
	iter = NodeSet.begin();
	for (long i = 0; i < num_cls; i++){ 
		string be = *iter; // get the 24-bit node

		// find all possible nodes with distance 1 from the given node
		for (int j = 0; j < num_bit; j++){
			string ber = be;
			ber[j] = ( 1- (ber[j] - '0') ) + '0'; // change 1 bit of the node

			if (NodeSet.find(ber) != NodeSet.end()){ // if the candidate node is in the set

				long idec = 0, jdec = 0;
				for (int k = 0; k < num_bit; k++){
					idec += (be[k] - '0') * (long) pow(2, num_bit - k - 1);
					jdec += (ber[k] - '0') * (long) pow(2, num_bit - k - 1);
				}
				
				if (!isCycle(parent, idec, jdec)){
					Union(parent, idec, jdec);
				}

			}
		}

		iter++; // get the next node
	}

	// union all the node pairs with distance 2
	
	// precompute index pair for changing 2 bits
	vector<pair<int, int>> BitPair;
	int bplen = num_bit * (num_bit - 1) / 2;
	BitPair.resize(bplen); // C^2_24
	int ct = 0;
	for (int j = 0; j < num_bit; j++){
		for (int k = 0; k < num_bit; k++){
			if (j != k && k > j){
				BitPair[ct] = make_pair(j, k);
				ct++;
			}
		}
	}

	iter = NodeSet.begin(); // reset the iterator
	for (long i = 0; i < num_cls; i++){
		string be = *iter;

		// find all possible nodes with distance 2 from the given node
		for (long p = 0; p < bplen; p++) {
			int j = BitPair[p].first;
			int k = BitPair[p].second;

			string ber = be;
			// change 2 bits of the node
			ber[j] = (1 - (ber[j] - '0')) + '0';
			ber[k] = (1 - (ber[k] - '0')) + '0';

			if (NodeSet.find(ber) != NodeSet.end()){ // if the candidate node is in the set

				long idec = 0, jdec = 0;
				for (int k = 0; k < num_bit; k++){
					idec += (be[k] - '0') * (long) pow(2, num_bit - k - 1);
					jdec += (ber[k] - '0') * (long) pow(2, num_bit - k - 1);
				}

				if (!isCycle(parent, idec, jdec)){
					Union(parent, idec, jdec);
				}
			}
		}

		iter++;
	}

	long maxCls = 0;
	for (long i = 0; i < num_parent; i++){
		if (parent[i] == -1){
			maxCls++;
		}
	}

	time = clock() - time;
	cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;
	cout << "max cluster number: " << maxCls << endl;

	delete[] parent;
	return 0;
}