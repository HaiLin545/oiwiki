#include <iostream>

using namespace std;

class Solution {
  public:
    bool isPowerOfTwo(int n) {
        if (n == 0)
            return false;
        if (n == 1) {
            return true;
        }
        if (n % 2 != 0)
            return false;
        return isPowerOfTwo(n / 2);
    }
};
int main() {
    int n;
    cin >> n;

    return 0;
}