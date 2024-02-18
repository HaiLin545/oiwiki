#include <iostream>
#include <vector>

using namespace std;

class Solution {
  public:
    void h(int n, vector<int> &A, vector<int> &B, vector<int> &C) {
        if (n == 1) {
            C.push_back(A.back());
            A.pop_back();
            return;
        }
        h(n - 1, A, C, B);
        C.push_back(A.back());
        A.pop_back();
        h(n - 1, B, A, C);
    }
    void hanota(vector<int> &A, vector<int> &B, vector<int> &C) {
        int n = A.size();
        h(n, A, B, C);
    }
};

int main() {

    vector<int> A;
    vector<int> B;
    vector<int> C;

    A.push_back(3);
    A.push_back(2);
    A.push_back(1);
    A.push_back(0);
    Solution s;
    s.hanota(A, B, C);
    return 0;
}