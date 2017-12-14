#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <stdio.h>

#define N 10000
using namespace std;


class mycomparison{
// write a comparison class that can be both greater (if reverse is true) and less (default)
// http://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
	bool reverse;
public:
	mycomparison(const bool& revparam = false){
		reverse = revparam;
	}
	// why use both 'const' and '&'? 
	// http://duramecho.com/ComputerInformation/WhyHowCppConst.html
	bool operator() (const int& a, const int& b) const{
		if (reverse) return (a > b);
		else return (a < b);
	}

};


void moveA2B(priority_queue<int, vector<int>, mycomparison>& A, priority_queue<int, vector<int>, mycomparison>& B){
// move one node from heap A to heap B
	if (~A.empty()){
		int Atop = A.top();
		A.pop();
		B.push(Atop);
	}
	else { perror("Heap A is empty"); }
}


int main(){

	int* num = new int[N];

	string mmfile = "Median.txt";
	string line, tmp;
	int i = 0;

	ifstream myfile(mmfile);
	if (myfile.is_open()){
		while (getline(myfile, line)){
			stringstream connection(line);
			connection >> tmp;

			num[i] = stoi(tmp);
			i++;
		}
	}

	// define heaps 
	typedef priority_queue<int, vector<int>, mycomparison> mypq_type;

	mypq_type less_max_heap; // less-than comparison, max heap
	mypq_type greater_min_heap(mycomparison(true)); // greater-than comparison, min heap


	less_max_heap.push(num[0]); 
	int sum = num[0];

	
	for (int i = 1; i < N; i++){
		(num[i] > less_max_heap.top()) ? greater_min_heap.push(num[i]) : less_max_heap.push(num[i]);

		if ((i + 1) % 2 == 0){

			while (less_max_heap.size() != greater_min_heap.size()){
				if (less_max_heap.size() > greater_min_heap.size()){
					moveA2B(less_max_heap, greater_min_heap);
				}
				else if (less_max_heap.size() < greater_min_heap.size()){
					moveA2B(greater_min_heap, less_max_heap);
				}
			}
			sum += less_max_heap.top();
		}
		else{
			less_max_heap.size() > greater_min_heap.size() ? sum += less_max_heap.top() : sum += greater_min_heap.top();
		}

	}


	cout << sum%10000 << endl;


	delete[] num;

	return 0;
}

