#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <assert.h>
using namespace std;

// solve 2SAT problem using an Strongly Connected Components (SCC) algorithm
//https://gist.github.com/HaiyangXu/190edfc1caaed95a9a01


class GraphElm{


    // direct initialization
    // https://msdn.microsoft.com/en-us/library/s16xw1a8.aspx#Anchor_2
    GraphElm(): isVisited(false) {}

    bool isVisited;

    vector<long> tails;
    int operator[](long index){ // operator overloading: https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.2.0/com.ibm.zos.v2r2.cbclx01/cplr318.htm
        // so that one can use graph[i][j], where i is a graph element index and j is a tail index
        assert(index < tails.size());
        return tails[index];
    }

};


typedef vector<GraphElm> Graph;  // https://en.wikipedia.org/wiki/Typedef

void GraphFromTxt(char *path, Graph) {




}