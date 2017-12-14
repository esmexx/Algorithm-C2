#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <algorithm>

using namespace std;

class ksnode {
private:
	long value;
	long weight;

public:
	ksnode() {};
	ksnode(long v, long w){
		value = v;
		weight = w;
	}

	long GetValue() const { return value; }
	long GetWeight() const { return weight; }

};


int main() {

	clock_t time;
	time = clock();

	string ksfile = "w4_knapsack_big.txt";
	string line, bwtmp, ntmp, vtmp, wtmp;
	int i = -1, BW, N;

	ksnode* num = NULL;

	ifstream myfile(ksfile);
	if (myfile.is_open()) {
		while (getline(myfile, line)){
			if (i == -1){
				stringstream connection(line);
				connection >> bwtmp >> ntmp;
				BW = stoi(bwtmp);
				N = stoi(ntmp);
				if (!num){
					num = new ksnode[N];
				}
				i++;
			}
			else{
				stringstream connection(line);
				connection >> vtmp >> wtmp;
				num[i] = ksnode(stoi(vtmp), stoi(wtmp));
				i++;
			}
		}
	}

	
	//int** V = new int*[N]; // get rid of the first column A[0,x]
	//for (int i = 0; i < N; i++){
	//	// change each column size based on W-wi, since cases are skipped when x < wi
	//	V[i] = new int[BW - num[i - 1].GetWeight() + 1];
	//}

	int Vx = BW + 1;
	// just keep 2 columns because only i-1 and i is needed
	int** V = new int*[2];
	for (int i = 0; i < 2; i++){
		V[i] = new int[Vx];
		if (i == 0){
			for (int j = 0; j < Vx; j++)  V[i][j] = (long) 0; // initialization A[0,x] = 0 for all x
		}
	} 

	for (int i = 1; i < N + 1; i++){
		int ic = i % 2; // current index, switch between col 0 and col 1 when computing A[i,x]
		int ip = (i - 1) % 2; // past index, also switch between col 0 and col 1
		for (int x = 0; x < Vx; x++){
			if (x - num[i - 1].GetWeight() >= 0){
				long a = V[ip][x];
				long b = V[ip][x - num[i - 1].GetWeight()] + num[i - 1].GetValue();
				V[ic][x] = max(a, b);
			}
			else{
				V[ic][x] = V[ip][x];
			}
		}
	}

	time = clock() - time;
	cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;

	V[0][BW] > V[1][BW] ? cout << V[0][BW] << endl : cout << V[1][BW] << endl;


	return 0;
}