#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
  public:
    map<int, vector<TreeNode *>> memo{};
    vector<TreeNode *> work(int n) {
        if (memo.find(n) != memo.end())
            return memo[n];
        vector<TreeNode *> res{};
        if (n == 1) {
            TreeNode *root = new TreeNode();
            res.push_back(root);
            return res;
        }
        n -= 1;
        for (int i = 1; i < n; i += 2) {
            vector<TreeNode *> left = work(i);
            vector<TreeNode *> right = work(n - i);
            for (int l = 0; l < left.size(); l++) {
                for (int r = 0; r < right.size(); r++) {
                    TreeNode *root = new TreeNode(0, left[l], right[r]);
                    res.push_back(root);
                }
            }
        }
        memo[n] = res;

        return res;
    }
    vector<TreeNode *> allPossibleFBT(int n) {
        vector<TreeNode *> vec{};
        if (n & 1 == 0)
            return vec;
        return work(n);
    }
};