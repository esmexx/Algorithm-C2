#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stdio.h>
#include <set>

using namespace std;

// heap element {job weight, job length}
class HEAP_ELM {
	// class member default as private
	int jobw;
	int jobl;

public:
	float greedyscore; // job schedule based on greedyscore

	HEAP_ELM() {};
	HEAP_ELM(int _jobw, int _jobl){
		jobw = _jobw;
		jobl = _jobl;
	}

	int getw() const { return jobw; }
	int getl() const { return jobl; }

	// 
	void calcscore_method1() {
		greedyscore = static_cast<float>(jobw)-static_cast<float>(jobl);
	}
	void calcscore_method2() {
		greedyscore = static_cast<float>(jobw) / static_cast<float>(jobl);
	}
};

// comparator for building min heap
class HEAP_COMP {
public:
	bool operator() (const HEAP_ELM& v1, const HEAP_ELM& v2){
		if (fabs(v1.greedyscore - v2.greedyscore) < std::numeric_limits<double>::epsilon()) {
			return v1.getw() > v2.getw(); // if difference is equal, job with higher weight go first
		}
		else {
			return v1.greedyscore > v2.greedyscore; // schedule jobs in decreasing order of the greedy score
		}
	}
};

int main(){

	string gsfile = "w1_jobs.txt";
	string line, tmp, wtmp, ltmp;
	HEAP_ELM* num = NULL;
	int i = 0, N;

	// elements in a set are unique, so the element is not inserted if it is already in the container
	// multiset allows duplicate elements
	multiset<HEAP_ELM, HEAP_COMP> gsched;

	// reading in txt file and building min heap simultaneously
	ifstream myfile(gsfile);
	if (myfile.is_open()){
		while (getline(myfile, line)){
			if (i == 0){
				stringstream connection(line);
				connection >> tmp;
				N = stoi(tmp);
				if (num == NULL){ num = new HEAP_ELM[N]; }
				i++;
			}
			else {
				stringstream connection(line);
				connection >> wtmp >> ltmp;
				num[i - 1] = HEAP_ELM(stoi(wtmp), stoi(ltmp));
				num[i - 1].calcscore_method1(); // method1: w-l, method2: w/l
				gsched.insert(num[i - 1]);
				i++;
			}
		}
	}


	long long int complt_time = 0, weighted_sum = 0;

	for (int i = 0; i < N; i++){
		complt_time += gsched.begin()->getl();
		weighted_sum += gsched.begin()->getw() * complt_time;
		gsched.erase(gsched.begin());
	}


	cout << "weighted sum is: " << weighted_sum << endl;

	delete[] num;

	return 0;

}