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
void getbitmask(int num_cities, int& num_subsets) {

    num_subsets = 1 << num_cities;
    int idx = 0; 
    int* bitmask = new int[num_subsets];

    for (int m = 1; m <= num_cities; m++){
        string strmask(m, 1);
        strmask.resize(num_cities, 0);

        do {
            int tmp = 0;
            for (int i = 0; i < num_cities; i++){
                if (strmask[i]){
                    tmp = (1 << i) | tmp;
                }
            }
            bitmask[idx] = tmp;
            idx++;
            //cout << tmp << endl;
        } while (prev_permutation(strmask.begin(), strmask.end()));
    }

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



    clock_t time;
    time = clock();

    int nss = 0;
    getbitmask(25, nss);

    time = clock() - time;
    cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;

    cout << "total number:" << nss << endl;

    for (int i = 0; i < ncities; i++){
        delete distmap[i];
    }
    delete[] distmap;
    
    delete[] num;

    return 0;

}