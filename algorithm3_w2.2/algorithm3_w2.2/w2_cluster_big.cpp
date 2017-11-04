#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>


using namespace std;



int main() {

	//string clsfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w2_clustering_big.txt";
	string clsfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\clustering2-example-200-12-solution-10.txt";
	//string clsfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\clustering2-example-200-12-solution-6.txt";
	string line, Ne, Nb, Te, TeStr;

	unordered_set<string> NodeSet;
	unordered_set<string> NodeMerged; // store all the nodes which get merged into the same cluster

	int Ncls = -1, Nbit = 0;
	ifstream myfile(clsfile);

	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if (Ncls == -1){
				stringstream connection(line);
				connection >> Ne >> Nb;
				Ncls = stoi(Ne);
				Nbit = stoi(Nb);
			}
			else {
				stringstream connection(line);
				TeStr = "";
				for (int i = 0; i < Nbit; i++){
					connection >> Te;
					TeStr += Te;
				}
				NodeSet.insert(TeStr); // insert string to the hash table 
			}
		}
	}

	Ncls = NodeSet.size(); // count unique nodes (since repeated nodes were ignored by unordered_set)

	unordered_set<string>::iterator iter = NodeSet.begin();
	unordered_set<string>::iterator posN, posM1, posM2;

	for (int i = 0; i < Ncls; i++){
		string be = *iter; // get the 24-bit node

		// find all possible nodes with distance 1 from the given node
		for (int j = 0; j < Nbit; j++){
			string ber = be;
			ber[j] = ( 1- (ber[j] - '0') ) + '0'; // change 1 bit of the node

			posM1 = NodeMerged.find(be);
			posM2 = NodeMerged.find(ber);

			// if the two nodes are not in the same cluster (i.e., they have not been merged)
			if (!(posM1 != NodeMerged.end() && posM2 != NodeMerged.end())) {
				posN = NodeSet.find(ber); // find the candidate node in the set
				if (posN != NodeSet.end()){ // if the candidate node presented in the set
					// merge it to the cluster
					NodeMerged.insert(ber);
					NodeMerged.insert(be);
				}
			}
		}

		iter++; // get the next node
	}


	// precompute index pair for changing 2 bits
	vector<pair<int, int>> BitPair;
	int bplen = Nbit * (Nbit - 1) / 2;
	BitPair.resize(bplen); // C^2_24
	int ct = 0;
	for (int j = 0; j < Nbit; j++){
		for (int k = 0; k < Nbit; k++){
			if (j != k && k > j){
				BitPair[ct] = make_pair(j, k);
				ct++;
			}
		}
	}

	iter = NodeSet.begin(); // reset the iterator
	for (int i = 0; i < Ncls; i++){
		string be = *iter;
		
		// find all possible nodes with distance 2 from the given node
		for (int p = 0; p < bplen; p++) {
			int j = BitPair[p].first;
			int k = BitPair[p].second;

			string ber = be;
			// change 2 bits of the node
			ber[j] = (1 - (ber[j] - '0')) + '0'; 
			ber[k] = (1 - (ber[k] - '0')) + '0';

			posM1 = NodeMerged.find(be);
			posM2 = NodeMerged.find(ber);

			// if the two nodes are not in the same cluster (i.e., they have not been merged)
			if (!(posM1 != NodeMerged.end() && posM2 != NodeMerged.end())) {
				posN = NodeSet.find(ber); // find the candidate node in the set
				if (posN != NodeSet.end()){ // if the candidate node presented in the set
					// merge it to the cluster
					NodeMerged.insert(ber);
					NodeMerged.insert(be);
				}
			}
		}

		iter++;

	}

	cout << "largest value of k needed is " << Ncls-NodeMerged.size()+1 << endl;

	return 0;
}