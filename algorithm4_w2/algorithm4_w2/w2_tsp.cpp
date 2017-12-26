#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <algorithm>

using namespace std;


double getdist(pair<double, double> src, pair<double, double> dest) {

    return sqrt(pow(src.first - dest.first, 2) + pow(src.second - dest.second, 2));
}

// generate all combinations (of size m-1) of the integers from 1 to n-1 in sorted order,
// where the first vertex (vertex 0) is always included
// e.g., 3 comb 5 gives 6 of such combinations
// http://rosettacode.org/wiki/Combinations#C.2B.2B
int* comb(int N, int M, long long& num_subsets){
    // generate each set S of size m that contains the first vertex
    M--;
    N--;

    string bitmask(M, 1);
    bitmask.resize(N, 0);

    long long numt = 1, det = 1;
    for (int i = 0; i < M; i++){
        numt = (N - i) * numt;
         det = (M - i) * det;
    }
    num_subsets = numt / det;

    // create an 1d array to store all the combinations
    int* subsets = new int[num_subsets * (M + 1)];

    int k = 0;
    do {
        subsets[k * (M + 1)] = 0; // the first element is always the first vertex 
        int j = 1;
        for (int i = 0; i < N; i++) {
            if (bitmask[i]) {
                subsets[k * (M + 1) + j] = i + 1;
                j++;
            }
        }
        k++;
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

    return subsets;

}

int main() {

    string tspfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 4\\w2_tsp_short.txt";
    string line, tnc, tx, ty;
    int i = -1, ncities;
    double locx, locy;

    pair<double, double>* num = NULL;

    // load the txt file and store the city coordinates
    ifstream myfile(tspfile);
    if (myfile.is_open()){
        while (getline(myfile, line)){
            if (i == -1){
                stringstream connection(line);
                connection >> tnc;
                ncities = stoi(tnc);
                if (num == NULL) { num = new pair<double, double>[ncities]; }
                i++;
            }
            else {
                stringstream connection(line);
                connection >> tx >> ty;
                locx = stod(tx);
                locy = stod(ty);

                num[i] = make_pair(locx, locy);
                i++;
            }
        }
    }

    // precompute distance between cities
    double** distmap = new double*[ncities];
    for (int i = 0; i < ncities; i++){
        distmap[i] = new double[ncities];
        for (int j = 0; j < ncities; j++){
            if (j != i){
                distmap[i][j] = getdist(num[i], num[j]);
            }
            else {
                distmap[i][j] = 0;
            }
        }
    }

    // dp algorithm from lecture slides
    int** A = new int*[ncities];
    for (int i = 0; i < ncities; i++){
        A[i] = new int[ncities];
        // initialize base case
        A[i][0] = numeric_limits<double>::max();
    }
    A[0][0] = 0;

    long long nss = 0; // number of subsets
    for (int m = 1; m < ncities; m++){ // m = subproblem size
        int* subsets = comb(ncities, m + 1, nss); // get all possible subsets with size m = 2, or 3, or,..., or n
        for (int n = 0; n < nss; n++){ // loop over each subset
            for (int k = 1; k < m + 1; k++){ // for each vertex j in the subset who is not the first vertex  
                int j = subsets[n * (m + 1) + k];
                
            }
        }
    }





    for (int i = 0; i < ncities; i++){
        delete A[i];
    }
    delete[] A;

    for (int i = 0; i < ncities; i++){
        delete distmap[i];
    }
    delete[] distmap;
    
    delete[] num;

    return 0;

}