#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include <time.h>
#include <algorithm>

#define N 1000000
using namespace std;

//http://www.voidcn.com/article/p-fiqcipui-bu.html

long long myhash(long long val){
	return val >> 15;
}

 

int main() {
	
	clock_t time;
	time = clock();

	// initialization
	long long* vallist = new long long[N]; // store numbers from txt file
	vector<long long> valnew; valnew.resize(6103503, -1);
	long long* tlist = new long long[20001];
	
	for (int i = 0; i < 20001; i++){
		tlist[i] = 0;
	}

	// read and convert txt file
	string txtfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 2\\prob-2sum.txt";
	string line, tmp;
	long long i = 0;
	
	ifstream myfile(txtfile);
	if (myfile.is_open()){
		while (getline(myfile, line)){
			stringstream connection(line);
			connection >> tmp;

			vallist[i] = stoll(tmp);
			i++;
		}
	}

	// perform hashing
	long long minval = *min_element(vallist, vallist + N), valkey;
	for (int i = 0; i < N; i++){
		valkey = myhash(vallist[i] - minval); // hashing on non-negative numbers
		valnew[valkey] == -1 ? (valnew[valkey] = vallist[i]) : valnew.push_back(vallist[i]);
	}
	long long valnew_len = valnew.size();

	// counting pairs
	long long firkey, seckey;
	for (int i = 0; i < N; i++){
		firkey = myhash(-10000 - vallist[i] - minval);
		seckey = myhash( 10000 - vallist[i] - minval);
		if (firkey < valnew_len){
			if (valnew[firkey] != -1){
				// valnew[firkey] is more than one number?
				if ((valnew[firkey] + vallist[i] >= -10000) && (valnew[firkey] + vallist[i] <= 10000))
					tlist[valnew[firkey] + vallist[i] + 10000] = 1; // ensure distictness?
			}
		}
		if (seckey < valnew_len){
			if (valnew[seckey] != -1){
				if ((valnew[seckey] + vallist[i] >= -10000) && (valnew[seckey] + vallist[i] <= 10000))
					tlist[valnew[seckey] + vallist[i] + 10000] = 1; // ensure distictness?
			}
		}
	}

	long long sumvalnew = accumulate(tlist, tlist + 20001, 0);

	time = clock() - time;
	std::cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;

	cout << "output is " << sumvalnew << endl;
	return 0;
}