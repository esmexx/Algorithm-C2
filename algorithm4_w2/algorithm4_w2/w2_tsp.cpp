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
int comb(int N, int M){
    int np = 0;
    string bitmask(M-1, 1);
    bitmask.resize(N-1, 0);

    do { // generate each set S of size m that contains the first vertex
        //cout << " 0";
        //for (int i = 0; i < N-1; i++){
        //    if (bitmask[i]) cout << " " << i+1;
        //}
        //cout << endl;
        np++;
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

    return np;

}

//int comb(int N, int M){
//    int np = 0;
//    string bitmask(M , 1);
//    bitmask.resize(N , 0);
//
//    do { // generate each set S of size m that contains the first vertex
//        if (bitmask[0]) {
//            //for (int i = 0; i < N; i++){
//            //    if (bitmask[i]) cout << " " << i;
//            //}
//            //cout << endl;
//            np++;
//        }
//    } while (prev_permutation(bitmask.begin(), bitmask.end()));
//
//    return np;
//
//}

int main() {

    string tspfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 4\\w2_tsp_short.txt";
    string line, tnc, tx, ty;
    int i = -1, ncities;
    double locx, locy;

    pair<double, double>* num = NULL;

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




    pair<double, double> S;

    for (int m = 1; m < ncities; m++){
    }

    clock_t time;
    time = clock();

    cout << comb(25, 12) << endl;

    time = clock() - time;
    cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;

    delete[] num;

    return 0;

}