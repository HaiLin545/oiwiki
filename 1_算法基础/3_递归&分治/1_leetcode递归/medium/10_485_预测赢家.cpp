#include <vector>
#include <iostream>

using namespace std;

// 递归
class Solution {
  public:
    int play(vector<int> &num, int start, int end) {
        if (start == end) {
            return num[start];
        }
        return max(num[start] - play(num, start + 1, end), num[end] - play(num, start, end - 1));
    }

    bool predictTheWinner(vector<int> &nums) { return play(nums, 0, nums.size() - 1) >= 0; }
};

// 动态规划

class Solution2 {
  public:
    bool predictTheWinner(vector<int> &nums) {
        int n = nums.size();
        auto dp = vector<vector<int>>(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            dp[i][i] = nums[i];
        for (int i = n - 2; i >= 0; i--) {
            for (int j = i + 1; j < n; j++) {
                dp[i][j] = max(nums[i] - dp[i + 1][j], nums[j] - dp[i][j - 1]);
            }
        }
        return dp[0][n - 1] >= 0;
    }
};