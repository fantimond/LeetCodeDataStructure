#include "AVL.hpp"

using namespace std;

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

        AVLNode* root = new AVLNode{2, 0}, *nd;
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
                nd = new AVLNode{v, 0};
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
	vector<int> result = sl.powerfulIntegers(2, 3, 10);
	for (auto v : result)
		cout << v << " ";
}


