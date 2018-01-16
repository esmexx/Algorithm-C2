#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <stack>
#include <algorithm>
#include <assert.h>
using namespace std;

// solve 2SAT problem using an Strongly Connected Components (SCC) algorithm
//https://gist.github.com/HaiyangXu/190edfc1caaed95a9a01


class GraphElm{
public:
    // direct initialization
    // https://msdn.microsoft.com/en-us/library/s16xw1a8.aspx#Anchor_2
    GraphElm(int l = -1, int f = -1) : isVisited(false), leader(l), fintime(f) {}
    
    int leader;
    int fintime;
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

bool cmp(pair<int, int>& a, pair<int, int>& b){
    return a.first < b.first;
}

class SCC{
    Graph _graph;
    Graph _graphrev;
    vector<pair<int, int>> finished;  // map finishing time to node index
    int total; // total visited
    int start; // start of DFS

public:

    void Compute(Graph &graph){
        _graph.swap(graph); // swap graph and _graph, similar to "cut and paste" graph to _graph
        GetReverseGraph(_graph);
        finished.resize(_graph.size());
        // Run DFS-Loop on Grev (first pass)
        DFSloop(_graphrev);
        Fin2Idx();
        // Run DFS-Loop on G, processing nodes in decreasing order of finishing times
        DFSloop(_graph, 2);
        CheckSCC();
    }

    void GetReverseGraph(Graph &graph){
        _graphrev.resize(graph.size());
        for (int i = 1; i < graph.size(); ++i){ // pre-increment: increment first, then evaluate
            for (int j = 0; j < graph[i].tails.size(); ++j){
                _graphrev[graph[i][j]].tails.push_back(i);
            }
        }
    }

    void DFSloop(Graph& graph, int pass = 1){
        
        total = 0;
        
        if (pass == 1){ // first pass
            int node = finished.size();
            while (--node > 0){
                if (!graph[node].isVisited){
                    start = node;
                    DFSstack(graph, start);
                }
            }
        }
        else { // second pass
            int node = finished.size();
            while (--node > 0){
                if (!graph[finished[node].second].isVisited){
                    start = finished[node].second;
                    DFSstack(graph, start);
                }
            }
        }
        

    }

    void DFSstack(Graph& graph, int node){
        // use stack instead of recursive call to do DFS
        graph[node].isVisited = true;
        graph[node].leader = node; // subject to change

        stack<int> S;
        S.push(node);

        while (!S.empty()){

            int v = S.top();
            int i;

            for (i = 0; i < graph[v].tails.size(); ++i){
                int w = graph[v][i];
                if (!graph[w].isVisited){
                    S.push(w);
                    graph[w].isVisited = true;
                    graph[S.top()].leader = node;
                    break; // once found a connected node, push the node to S and start finding its children
                }
            }

            if (i == graph[v].tails.size()){ // if all the children of the current node is found, then the current node will be the next finished node
                ++total;
                graph[S.top()].fintime = total; // finish time
                S.pop();
            }
        }
    }

    void Fin2Idx(){ // get node index from finish time
        for (int i = 1; i < _graphrev.size(); ++i){
            finished[i] = make_pair(_graphrev[i].fintime, i);
        }
        sort(finished.begin(), finished.end(), cmp);
    }

    void CheckSCC(){
        bool satisfy2SAT = true;
        for (int i = 1; i <=size; ++i){
            if (_graph[i].leader == _graph[i + size].leader){
                satisfy2SAT = false;
                break;
            }
        }
        if (satisfy2SAT) cout << "2SAT Satisfy" << endl;
        else cout << "2SAT Unsatisfy" << endl;
    }

};


int main() {

    clock_t time;
    time = clock();

    Graph graph;
    
    GraphFromTxt("2sat6.txt", graph);

    SCC scc;
    if (graph.size()>2){
        scc.Compute(graph);
    }

    time = clock() - time;
    std::cout << "Time spent:" << (float)time / CLOCKS_PER_SEC << "seconds" << endl;

    return 0;
}