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


// modified from http://rosettacode.org/wiki/Combinations#C.2B.2B
void getbitmask(long num_cities, long num_subsets, long* & subset_bitmasks, pair<long, long>* & subset_interval_indices) {

    long idx = 0; // index counter for subsets
    subset_interval_indices[0] = make_pair(-1, -1); // subset of size 0 does not exist

    for (long m = 1; m <= num_cities; m++){
        // initialize bitmask to represent subset of size m
        string strmask(m, 1);
        strmask.resize(num_cities, 0);

        long interval_left = idx; // record starting position of the interval

        // permute the bitmask thoroughly
        do {
            long tmp = 0;
            for (long i = 0; i < num_cities; i++){
                if (strmask[i]){
                    tmp = (1 << i) | tmp;
                }
            }
            subset_bitmasks[idx] = tmp;
            idx++;
        } while (prev_permutation(strmask.begin(), strmask.end()));

        subset_interval_indices[m] = make_pair(interval_left, idx); // the interval range is [left, idx)
    }

}


int main() {

    string tspfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 4\\w2_tsp_short.txt";
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
                distmap[i][j] = getdist(num[i], num[j]);
            }
            else {
                distmap[i][j] = 0;
            }
        }
    }


    // the bitmask dynamic programming solution to Traveling Salesman
    // http://www.cs.ucf.edu/~dmarino/progcontests/modules/dptsp/DP-TSP-Notes.pdf

    long nsubsets = (1 << ncities) - 1; // number of possible subsets: 2^n - 1
    long* bitmask = new long[nsubsets]; // all possible subsets of size m = 1,2,3,... num_cities are presented in terms of bitmask
    pair<long, long>* sindices = new pair<long, long>[ncities]; // stores the beginning and ending position of subset S of the same size in the bitmask array, [a, b)

    getbitmask(ncities, nsubsets, bitmask, sindices);





    // delete allocated arrays
    delete[] num;

    for (long i = 0; i < ncities; i++){
        delete distmap[i];
    }
    delete[] distmap;

    delete[] bitmask;
    delete[] sindices;

    return 0;

}