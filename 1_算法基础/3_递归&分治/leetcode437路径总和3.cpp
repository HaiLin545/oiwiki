#include <iostream>
#include <string>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 从该节点出发，和为sum的路径数
long count(TreeNode *node, int sum) {
    if (node == nullptr)
        return 0;
    return (node->val == sum) + count(node->left, sum - node->val) +
           count(node->right, sum - node->val);
}

long pathSum(TreeNode *root, long targetSum) {
    if (root == NULL)
        return 0;
    return count(root, targetSum) + pathSum(root->left, targetSum) +
           pathSum(root->right, targetSum);
}

int main(int argc, char *argv[]) {
    const int len = atoi(argv[1]);
    string arr[len];
    for (int i = 0; i < len; i++) {
        arr[i] = argv[i + 2];
    }
    TreeNode *tn[len];
    for (int i = 0; i < len; i++) {
        tn[i] = (arr[i] == "null") ? NULL : new TreeNode(atoi(arr[i].c_str()));
    }
    for (int i = 0; i < len; i++) {
        if (i * 2 + 1 >= len) {
            break;
        }
        tn[i]->left = tn[i * 2 + 1];
        tn[i]->right = tn[i * 2 + 2];
    }
    int target = atoi(argv[len + 2]);
    int ans = pathSum(tn[0], target);
    cout << "ans = " << ans << endl;
    return 0;
}
