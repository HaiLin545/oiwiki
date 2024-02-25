#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
  public:
    int calcu(int a, int b, char op) {
        if (op == '+')
            return a + b;
        if (op == '-')
            return a - b;
        return a * b;
    }
    vector<int> diffWaysToCompute(string expression) {
        int len = expression.size();

        vector<int> res{};
        int op_cnt = 0;
        for (int i = 0; i < len; i++) {
            if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*') {
                op_cnt++;
                string exp_left = expression.substr(0, i);
                string exp_right = expression.substr(i + 1);
                vector<int> left_res = diffWaysToCompute(exp_left);
                vector<int> right_res = diffWaysToCompute(exp_right);
                for (int u = 0; u < left_res.size(); u++) {
                    for (int v = 0; v < right_res.size(); v++) {
                        res.push_back(calcu(left_res[u], right_res[v], expression[i]));
                    }
                }
            }
        }
        if (op_cnt == 0) {
            res.push_back(stoi(expression));
        }
        return res;
    }
};

int main() {
    Solution s;
    vector<int> a = s.diffWaysToCompute("2-1-1");
    return 0;
}