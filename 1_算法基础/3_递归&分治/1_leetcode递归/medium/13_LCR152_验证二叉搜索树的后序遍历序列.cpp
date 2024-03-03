
#include <vector>
using namespace std;

class Solution {
  public:
    bool work(vector<int> &postorder, int start, int end) {
        int len = end - start + 1;
        if (len <= 1)
            return true;
        int p = start;
        int q = end - 1;
        while (p < end && postorder[p] < postorder[end]) {
            p++;
        }
        while (q >= 0 && postorder[q] > postorder[end]) {
            q--;
        }
        return (p == q + 1) && work(postorder, start, p - 1) && work(postorder, p, end - 1);
    }
    bool verifyTreeOrder(vector<int> &postorder) {
        return work(postorder, 0, postorder.size() - 1);
    }
};