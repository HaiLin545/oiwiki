#include <unordered_map>

class Solution {
  public:
    int mod = 1e9 + 7;
    std::unordered_map<long long, long long> mp;
    long long count(long long n) {
        if (mp[n])
            return mp[n];
        if (n == 1)
            return 5;
        if (n == 2)
            return 20l;
        long long m = n / 2;
        if (m & 1)
            m += 1;
        long long c1 = count(m);
        mp[m] = c1;
        long long c2 = count(n - m);
        mp[n - m] = c2;
        return c1 * c2 % mod;
    }
    int countGoodNumbers(long long n) { return count(n); }
};