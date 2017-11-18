#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdio.h>

using namespace std;

class ksnode {
private:
	int value;
	int weight;

public:
	ksnode() {};
	ksnode(int v, int w){
		value = v;
		weight = w;
	}

	int GetValue() const { return value; }
	int GetWeight() const { return weight; }

};


int main() {

	string ksfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w4_knapsack.txt";
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

	// create 2d array for storing values
	int Vi = N + 1;
	int Vx = BW + 1;

	int** V = new int*[Vi];
	for (int i = 0; i < Vi; i++){
		V[i] = new int[Vx];
		if (i == 0){
			for (int x = 0; x < Vx; x++)  V[i][x] = 0; // initialization A[0,x] = 0 for all x
		}
	}

	for (int i = 1; i < Vi; i++){
		for (int x = 0; x < Vx; x++){
			if (x - num[i - 1].GetWeight() >= 0){
				int a = V[i - 1][x];
				int b = V[i - 1][x - num[i - 1].GetWeight()] + num[i - 1].GetValue();
				V[i][x] = max(a, b);
			}
			else {
				V[i][x] = V[i - 1][x];
			}
		}
	}

	cout << V[N][BW] << endl;

	return 0;
}