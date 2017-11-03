#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>


using namespace std;



int main() {

	string clsfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w2_clustering_big.txt";
	string line, Ne, Nb, Te, TeStr;

	unordered_set<string> NodeSet;
	unordered_set<string> NodeMerged;

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

	Ncls = NodeSet.size();
	int maxNcls = Ncls;

	unordered_set<string>::iterator iter = NodeSet.begin();
	unordered_set<string>::iterator pos, pos1, pos2;

	for (int i = 0; i < Ncls; i++){
		string be = *iter; // get the 24-bit node

		// find all possible nodes with distance 1 from the given node
		for (int j = 0; j < Nbit; j++){
			string ber = be;
			ber[j] = ( 1- (ber[j] - '0') ) + '0'; // change 1 bit of the node
			pos = NodeSet.find(ber);

			// if the candidate node is in the hash table, "merge" it to the given node
			if (pos != NodeSet.end()){
				// store the node to another hash table (alternative to merge) 
				NodeMerged.insert(*pos); 
				NodeMerged.insert(be); 
				maxNcls--;
			}
		}

		iter++;
	}

	iter = NodeSet.begin(); // reset the iterator

	
	vector<pair<int, int>> BitPair;
	BitPair.resize(Nbit * (Nbit-1) / 2); // C^2_24
	int ct = 0;
	for (int j = 0; j < Nbit; j++){
		for (int k = 0; k < Nbit; k++){
			if (j != k && k > j){
				BitPair[ct] = make_pair(j, k);
				ct++;
			}
		}
	}



	for (int i = 0; i < Ncls; i++){
		string be = *iter;
		
		// find all possible nodes with distance 2 from the given node
		for (int p = 0; p < (Nbit*(Nbit - 1) / 2); p++) {
			int j = BitPair[p].first;
			int k = BitPair[p].second;

			string ber = be;
			// change 2 bits of the node
			ber[j] = (1 - (ber[j] - '0')) + '0'; 
			ber[k] = (1 - (ber[k] - '0')) + '0';

			pos1 = NodeSet.find(ber);
			pos2 = NodeMerged.find(ber); // check for nodes that has already been merged

			// if the candidate node is in the hash table, and has not been merged before, "merge" it
			if (pos1 != NodeSet.end() && pos2 == NodeMerged.end()) {
				NodeMerged.insert(*pos1);
				NodeMerged.insert(be);
				maxNcls--;
			}
		}

		iter++;

	}

	cout << "largest value of k needed is " << maxNcls << endl;

	return 0;
}