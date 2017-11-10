#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <set>
#include <unordered_set>
#include <stdio.h>
#include <math.h>

using namespace std;


class TreeNode {
private:
	long long weight;
	TreeNode* left;
	TreeNode* right;

public:
	TreeNode();

	// create a node from scratch
	TreeNode(long long _weight){
		weight = _weight;
		left = right = NULL;
	}

	// create meta node by combining left and right node
	TreeNode(TreeNode* _left, TreeNode* _right){
		weight = _left->GetWeight() + _right->GetWeight();
		left = _left;
		right = _right;
	}

	long long GetWeight() const { return weight; }
	TreeNode* GetLeft() const { return left; }
	TreeNode* GetRight() const { return right; }

};

class TreeComp {
public:
	bool operator() (const TreeNode& n1, const TreeNode& n2){
		return n2.GetWeight() > n1.GetWeight(); // for building min heap
	}
};



int main() {

	string hfmfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w3_huffman_short.txt";
	string line, ntmp, wtmp;
	int i = 0, num_nodes;

	multiset<TreeNode, TreeComp> hfm_tree;

	ifstream myfile(hfmfile);
	if (myfile.is_open()){
		while (getline(myfile, line)){
			if (i == 0){
				stringstream connection(line);
				connection >> ntmp;
				num_nodes = stoi(ntmp);
				i++;
			}
			else {
				stringstream connection(line);
				connection >> wtmp;
				hfm_tree.insert(TreeNode(stoi(wtmp)));
			}
		}
	}


	multiset<long long> not_visited; // store all the tree nodes and metanodes as not visited

	while (hfm_tree.size() > 1){
		// get first two nodes from the heap and combine them into a metanode, 
		// then insert the metanode back to the heap

		TreeNode *ltmp, *rtmp;

		if (hfm_tree.begin()->GetLeft() != NULL && hfm_tree.begin()->GetRight() != NULL){
			// if the node on the top is not a "stand-alone" node, then create new tree 
			// node inheriting from all children nodes
			ltmp = new TreeNode(hfm_tree.begin()->GetLeft(), hfm_tree.begin()->GetRight());
			not_visited.insert(ltmp->GetWeight());
		}
		else {
			ltmp = new TreeNode(hfm_tree.begin()->GetWeight());
			not_visited.insert(ltmp->GetWeight());
		}
		hfm_tree.erase(hfm_tree.begin());

		if (hfm_tree.begin()->GetLeft() != NULL && hfm_tree.begin()->GetRight() != NULL){
			rtmp = new TreeNode(hfm_tree.begin()->GetLeft(), hfm_tree.begin()->GetRight());
			not_visited.insert(rtmp->GetWeight());
		}
		else {
			rtmp = new TreeNode(hfm_tree.begin()->GetWeight());
			not_visited.insert(rtmp->GetWeight());
		}
		hfm_tree.erase(hfm_tree.begin());

		// create a metanode
		TreeNode MetaNode(ltmp, rtmp);

		// insert the node to the heap
		hfm_tree.insert(MetaNode);

	}

	// get the root from heap
	TreeNode* root = new TreeNode(hfm_tree.begin()->GetLeft(), hfm_tree.begin()->GetRight());
	not_visited.insert(root->GetWeight());

	// use a BFS approach to compute node height
	queue<TreeNode*> tnq;
	int hfm_len = 0;


	tnq.push(root);
	// mark root as visited, i.e. remove node from unvisited
	if (not_visited.find(root->GetWeight()) != not_visited.end()){
		not_visited.erase(not_visited.equal_range(root->GetWeight()).first);
	}

	while (!tnq.empty()) {
		TreeNode* v = tnq.front();
		tnq.pop();

		// mark v as visited, i.e. remove node from unvisited
		if (not_visited.find(v->GetWeight()) != not_visited.end()){
			not_visited.erase(not_visited.equal_range(v->GetWeight()).first);
		}


		// process all children nodes of v
		TreeNode* lv = v->GetLeft();

		// if lv has not been visited   
		// cannot handle repeated node weight!!
		if (lv != NULL) {
			if (not_visited.find(lv->GetWeight()) != not_visited.end()) {
				tnq.push(lv);
				//mark left node to be visited
				not_visited.erase(not_visited.equal_range(lv->GetWeight()).first);
			}
		}

		// what if rv is NULL

		TreeNode* rv = v->GetRight();
		if (rv != NULL) {
			if (not_visited.find(rv->GetWeight()) != not_visited.end()){
				tnq.push(rv);
				not_visited.erase(not_visited.equal_range(rv->GetWeight()).first);
			}
		}

	}


	return 0;
}