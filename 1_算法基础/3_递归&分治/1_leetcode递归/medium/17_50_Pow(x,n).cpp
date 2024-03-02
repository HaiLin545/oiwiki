class Solution {
  public:
    double myPow(double x, int n) {
        if (x == 1.0)
            return 1;
        if (n == 0)
            return 1.0;
        if (n < 0)
            return myPow(1.0 / x, -(n + 1)) / x;
        if (n == 1)
            return x;
        if (n & 1)
            return x * myPow(x, n - 1);
        double x2 = x * x;
        return x2 * myPow(x2, (n - 2) / 2);
    }
};