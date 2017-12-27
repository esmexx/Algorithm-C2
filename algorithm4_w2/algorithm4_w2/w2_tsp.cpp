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

double GetDist(pair<double, double> src, pair<double, double> dest) {
    return sqrt(pow(src.first - dest.first, 2) + pow(src.second - dest.second, 2));
}

int main() {

    string tspfile = "w2_tsp.txt";
    string line, tnc, tx, ty;
    long i = -1, ncities;
    double locx, locy;

    pair<double, double>* num = NULL;

    // load the txt file and store the city coordinates
    ifstream myfile(tspfile);
    if (myfile.is_open()){
        while (getline(myfile, line)){
            if (i == -1){
                stringstream connection(line);
                connection >> tnc;
                ncities = stol(tnc);
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
    for (long i = 0; i < ncities; i++){
        distmap[i] = new double[ncities];
        for (long j = 0; j < ncities; j++){
            if (j != i){
                distmap[i][j] = GetDist(num[i], num[j]);
            }
            else {
                distmap[i][j] = 0;
            }
        }
    }


    // the bitmask dynamic programming solution to Traveling Salesman
    long nsubsets = 1 << ncities; // number of possible subsets: 2^n
    long bitmask;

    double** A = new double*[nsubsets];
    for (long i = 0; i < nsubsets; i++) { 
        A[i] = new double[ncities]; 
        for (long j = 0; j < ncities; j++)
            A[i][j] = DBL_MAX; // some initialization
    }

    // fulfill base case
    A[1][0] = 0; // A[S, 1] = 0 if S = {1}

    for (long m = 2; m <= ncities; m++){ // m = subproblem size
        string strmask(m, 1);
        strmask.resize(ncities, 0);

        do {
            if (strmask[0]){ // for each set S of size m that contains 1
                bitmask = 1;
                for (long i = 1; i < ncities; i++){
                    if (strmask[i]){
                        bitmask = (1 << i) | bitmask;
                    }
                }

                // for each j belonged to S, where j != 1
                for (int j = 1; j < ncities; j++){ // exclude city 1 from the choice of j
                    if (((bitmask >> j) & 1) == 1){ // check what j is in the subset S
                        long bitmask_nj = bitmask - (1 << j); // remove j from the subset
                        for (int k = 0; k < ncities; k++){
                            if (((bitmask_nj >> k) & 1) == 1){ // for all the k in the subset, where k != j
                                if (A[bitmask_nj][k] + distmap[k][j] < A[bitmask][j]) // A[S, j] = min{A[S-{j}, k] + cost[k,j]} for k in S and k != j
                                    A[bitmask][j] = A[bitmask_nj][k] + distmap[k][j];
                            }
                        }
                    }
                }

            }
        } while (prev_permutation(strmask.begin(), strmask.end()));
    }

    // get the minDist from min{A[{1,2,...,n}, j]+cost[j,1]}
    double minDist = DBL_MAX;
    for (long j = 1; j < ncities; j++){
        if (A[nsubsets-1][j] + distmap[j][0] < minDist)
            minDist = A[nsubsets-1][j] + distmap[j][0];
    }

    cout << minDist << endl;



    // delete allocated arrays
    delete[] num;

    for (long i = 0; i < ncities; i++){
        delete distmap[i];
    }
    delete[] distmap;

    for (long i = 0; i < nsubsets; i++){
        delete A[i];
    }
    delete[] A;

    return 0;

}