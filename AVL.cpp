#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// AVL Tree Node, deletion is not implemented yet
class Node {
public:
    int val;
    int bf;
    Node* parent;
    Node* left;
    Node* right;
    Node* Node::*ac_left;
    Node* Node::*ac_right;

    int isSide() {
        return parent->right == this?1:-1;
    }

    int get_bf_i() {
    	return bf;
    }
    int get_bf_m() {
    	return -bf;
    }
    int (Node::*get_bf)();

    void set_bf_i(int v) {
    	bf = v;
    }
    void set_bf_m(int v) {
    	bf = -v;
    }
    void (Node::*set_bf)(int);

    Node* insert(Node* nd) {
    	int v = nd->val;
    	auto anchor = this, root = this;
        while(true) {    // insert node
        	if(v > anchor->val) {
        		ac_right = &Node::right;
        	}
        	else if(v < anchor->val) {
        		ac_right = &Node::left;
        	}
            else {
                delete nd;
                return root;
            }

			if(anchor->*ac_right) {
				anchor = anchor->*ac_right;
			}
			else {
				anchor->*ac_right = nd;
				nd->parent = anchor;
				break;
			}
        }


        anchor = nd;    // fix AVL property
        while(anchor->parent) {
            int side = anchor->isSide();
            if (anchor->parent->bf == -side) {
                anchor->parent->bf = 0;
                return root;
            }
            else if (anchor->parent->bf == 0) {
                anchor->parent->bf = side; // height += 1;
                anchor = anchor->parent;
                continue; // move up
            }
            else {
                anchor->parent->bf += side; // 2 or -2, need rotation
                bool left = true, right = false, r1, r2;
                if (anchor->parent->bf == 2) {
                	r1 = right;
                	r2 = left;
                }
                else if (anchor->parent->bf == -2) {
                	r1 = left;
                	r2 = right;
                }
                else {
                	cerr << "anchor->parent->bf = " << anchor->parent->bf << endl;
                	exit(1);
                }

                if (anchor->parent->bf * anchor->bf == -2){
                    anchor = anchor->rotate(r1);
                    anchor = anchor->parent->rotate(r2);
                }
                else {
                	anchor = anchor->parent->rotate(r2);
                }
                if (anchor->parent)
                	return root;
                else
                	return anchor;
            }
        }
        return root;
    }

    Node* rotate(bool isLeft=true) {
    	if (isLeft) {
			ac_left = &Node::left;
			ac_right = &Node::right;
			get_bf = &Node::get_bf_i;
			set_bf = &Node::set_bf_i;
    	}
    	else {
    		ac_left = &Node::right;
    		ac_right = &Node::left;
    		get_bf = &Node::get_bf_m;
    		set_bf = &Node::set_bf_m;
    	}

        Node* parent = this->parent, *A = this, *B = this->*ac_right;
        int a_bf = (A->*get_bf)(), b_bf = (B->*get_bf)();

        // graft 1
        if (parent) {
            int side = this->isSide();
            if (side == 1) {
                parent->right = B;
            }
            else {
                parent->left = B;
            }
        }
        B->parent = parent;

        // graft 2
        if (B->*ac_left) {
            (B->*ac_left)->parent = A;
        }
        A->*ac_right = B->*ac_left;

        // graft 3
        A->parent = B;
        B->*ac_left = A;

        if(a_bf == 2 && b_bf == 1) { // pure leftRotate and one case of right-left
            (A->*set_bf)(0);
            (B->*set_bf)(0);
        }
        else if(a_bf == 1) { // left-right
            if(b_bf == -1){
            	(A->*set_bf)(0);
            	(B->*set_bf)(-2);
            }
            else if(b_bf == 0){
            	(A->*set_bf)(0);
            	(B->*set_bf)(-1);
            }
            else if(b_bf == 1){
            	(A->*set_bf)(-1);
            	(B->*set_bf)(-1);
            }
            else {
            	cerr << "a_bf: " << a_bf << ", b_bf: " << b_bf << endl;
            	exit(1);
            }
        }
        else if(a_bf == 2 && b_bf == 2) { // one case of right-left
        	(A->*set_bf)(-1);
            (B->*set_bf)(0);
        }
        else {
        	cerr << "a_bf: " << a_bf << ", b_bf: " << b_bf << endl;
        	exit(1);
        }

        return B;

    }

    void serialize(vector<int>& output) {
    	output.push_back(val);
    	if (left) {
    		left->serialize(output);
    	}
    	if (right) {
    		right->serialize(output);
    	}
    }


};


// Application:
// Given two non-negative integers x and y, an integer is powerful if it is equal to x^i + y^j for some integers i >= 0 and j >= 0.
// Return a list of all powerful integers that have value less than or equal to bound.
// You may return the answer in any order.  In your answer, each value should occur at most once.
//
//
// Example 1:
//
// Input: x = 2, y = 3, bound = 10
// Output: [2,3,4,5,7,9,10]
// Explanation:
// 2 = 2^0 + 3^0
// 3 = 2^1 + 3^0
// 4 = 2^0 + 3^1
// 5 = 2^1 + 3^1
// 7 = 2^2 + 3^1
// 9 = 2^3 + 3^0
// 10 = 2^0 + 3^2
// Example 2:
//
// Input: x = 3, y = 5, bound = 15
// Output: [2,4,6,8,10,14]

class Solution {
public:

    vector<int> powerfulIntegers(int x, int y, int bound) {
        // AVL tree
        vector<int> X, Y;
        int v = 1;
        if (x != 1) {
			while(v <= bound) {
				X.push_back(v);
				v *= x;
			}
        }
        else {
        	X.push_back(1);
        }
        v = 1;
        if (y != 1) {
			while(v <= bound) {
				Y.push_back(v);
				v *= y;
			}
        }
        else {
        	Y.push_back(1);
        }

        Node* root = new Node{2, 0}, *nd;
        if (bound < 2) {
        	return vector<int>();
        }
        int sizeX = X.size(), sizeY = Y.size();
        int jhead = 1;
        for (int i = 0; i < sizeX; i++) {
            for (int j = jhead; j < sizeY; j++) {
                v = X[i] + Y[j];
                if (v > bound) {
                	continue;
                }
                nd = new Node{v, 0};
                root = root->insert(nd);
            }
            jhead = 0;
        }

        vector<int> output;
        root->serialize(output);
        return output;
    }
};

int main() {
	Solution sl;
	vector<int> result = sl.powerfulIntegers(2, 3, 200);
	for (auto v : result)
		cout << v << " ";
}


