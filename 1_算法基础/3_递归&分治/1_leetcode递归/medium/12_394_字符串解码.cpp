#include <map>
#include <string>
#include <stack>
using namespace std;

class Solution {
  public:
    map<int, int> m;
    stack<int> sta;
    bool isDigit(char s) { return s >= '0' && s <= '9'; }
    bool isCharacter(char s) { return s >= 'a' && s <= 'z'; }
    string work(string &s, int p, int q) {
        int len = s.size();
        string res = "";
        while (p <= q) {
            if (isCharacter(s[p])) {
                int start = p;
                while (isCharacter(s[p]) && p <= q) {
                    p++;
                }
                res += s.substr(start, p - start);
                continue;
            }
            if (isDigit(s[p])) {
                int num = 0;
                while (isDigit(s[p])) {
                    num = num * 10 + (s[p] - '0');
                    p++;
                }
                string sub = work(s, p + 1, m[p] - 1);
                for (int i = 0; i < num; i++) {
                    res += sub;
                }
                p = m[p] + 1;
            }
        }
        return res;
    }
    string decodeString(string s) {
        int len = s.size();
        for (int i = 0; i < len; i++) {
            if (s[i] == '[') {
                sta.push(i);
            }
            if (s[i] == ']') {
                m[sta.top()] = i;
                sta.pop();
            }
        }
        return work(s, 0, s.size() - 1);
    }
};