#ifndef AVL_CLASS
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>


// AVL Tree AVLNode, deletion is not implemented yet
class AVLNode {
public:
    int val;
    int bf;
    AVLNode* parent;
    AVLNode* left;
    AVLNode* right;
    AVLNode* AVLNode::*ac_left;
    AVLNode* AVLNode::*ac_right;

    int isSide() {
        return parent->right == this?1:-1;
    }

    int get_bf_i() {
    	return bf;
    }
    int get_bf_m() {
    	return -bf;
    }
    int (AVLNode::*get_bf)();

    void set_bf_i(int v) {
    	bf = v;
    }
    void set_bf_m(int v) {
    	bf = -v;
    }
    void (AVLNode::*set_bf)(int);

    AVLNode* insert(AVLNode* nd) {
    	int v = nd->val;
    	auto anchor = this, root = this;
        while(true) {    // insert AVLNode
        	if(v > anchor->val) {
        		ac_right = &AVLNode::right;
        	}
        	else if(v < anchor->val) {
        		ac_right = &AVLNode::left;
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
                	std::cerr << "anchor->parent->bf = " << anchor->parent->bf << std::endl;
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

    AVLNode* rotate(bool isLeft=true) {
    	if (isLeft) {
			ac_left = &AVLNode::left;
			ac_right = &AVLNode::right;
			get_bf = &AVLNode::get_bf_i;
			set_bf = &AVLNode::set_bf_i;
    	}
    	else {
    		ac_left = &AVLNode::right;
    		ac_right = &AVLNode::left;
    		get_bf = &AVLNode::get_bf_m;
    		set_bf = &AVLNode::set_bf_m;
    	}

        AVLNode* parent = this->parent, *A = this, *B = this->*ac_right;
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
            	std::cerr << "a_bf: " << a_bf << ", b_bf: " << b_bf << std::endl;
            	exit(1);
            }
        }
        else if(a_bf == 2 && b_bf == 2) { // one case of right-left
        	(A->*set_bf)(-1);
            (B->*set_bf)(0);
        }
        else {
        	std::cerr << "a_bf: " << a_bf << ", b_bf: " << b_bf << std::endl;
        	exit(1);
        }

        return B;

    }

    void serialize(std::vector<int>& output) {
    	output.push_back(val);
    	if (left) {
    		left->serialize(output);
    	}
    	if (right) {
    		right->serialize(output);
    	}
    }


};

#endif

