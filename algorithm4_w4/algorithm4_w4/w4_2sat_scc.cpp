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
public:
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
int size;

void GraphFromTxt(char *path, Graph& graph) {

    ifstream input(path);
    if (input.is_open()){

        int head, tail, nhead, ntail;
        input >> size;
        graph.push_back(GraphElm()); // skipe index 0 and start from 1
        graph.resize(2*size + 1);
        while (input >> head >> tail){
            // all u's range from 1 to size, and all u_bar's range from size+1 to 2*size+1 
            if (head < 0){
                nhead = -head;
                head = size - head;
            }
            else {
                nhead = size + head;
            }

            if (tail < 0){
                ntail = -tail;
                tail = size - tail;
            }
            else{
                ntail = size + tail;
            }
            // for each clause (u OR v), we add edges u_bar -> v and v_bar -> u to graph
            graph[nhead].tails.push_back(tail);
            graph[ntail].tails.push_back(head);
        }
    }

}

int main() {

    Graph graph;
    
    GraphFromTxt("2sat1.txt", graph);


}