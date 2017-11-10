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


//https://stackoverflow.com/questions/17637336/how-to-build-a-binary-tree-from-the-leaves-to-the-root
// https://gist.github.com/mgechev/5911348
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


// https://articles.leetcode.com/maximum-height-of-binary-tree/
int maxHeight(TreeNode* p) {

	if (!p) return 0;
	long long left_height = maxHeight(p->GetLeft());
	long long right_height = maxHeight(p->GetRight());

	return (left_height > right_height) ? left_height + 1 : right_height + 1;

}

// https://discuss.leetcode.com/topic/8723/my-4-line-java-solution/3
int minHeight(TreeNode* p) {
	if (!p) return 0;
	if (!p->GetLeft()) {
		return minHeight(p->GetRight()) + 1;
	}
	if (!p->GetRight()){
		return minHeight(p->GetLeft()) + 1;
	}

	return min(minHeight(p->GetLeft()), minHeight(p->GetRight())) + 1;
}


int main() {

	string hfmfile = "C:\\Users\\Xiaoxuan\\Desktop\\cousera\\algorithm stanford\\course 3\\w3_huffman.txt";
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


	while (hfm_tree.size() > 1){
		// get first two nodes from the heap and combine them into a metanode, 
		// then insert the metanode back to the heap

		TreeNode *ltmp, *rtmp;

		if (hfm_tree.begin()->GetLeft() != NULL && hfm_tree.begin()->GetRight() != NULL){
			// if the node on the top is not a "stand-alone" node, then create new tree 
			// node inheriting from all children nodes
			ltmp = new TreeNode(hfm_tree.begin()->GetLeft(), hfm_tree.begin()->GetRight());
		}
		else {
			ltmp = new TreeNode(hfm_tree.begin()->GetWeight());
		}
		hfm_tree.erase(hfm_tree.begin());

		if (hfm_tree.begin()->GetLeft() != NULL && hfm_tree.begin()->GetRight() != NULL){
			rtmp = new TreeNode(hfm_tree.begin()->GetLeft(), hfm_tree.begin()->GetRight());
		}
		else {
			rtmp = new TreeNode(hfm_tree.begin()->GetWeight());
		}
		hfm_tree.erase(hfm_tree.begin());

		// create a metanode
		TreeNode MetaNode(ltmp, rtmp);

		// insert the node to the heap
		hfm_tree.insert(MetaNode);

	}

	// get the root from heap
	TreeNode* root = new TreeNode(hfm_tree.begin()->GetLeft(), hfm_tree.begin()->GetRight());

	
	cout << "max height is: " << maxHeight(root) - 1 << endl; // the furthese node with weight is the parent node of the NULL node
	cout << "min height is: " << minHeight(root) - 1 << endl;

	
	return 0;
}