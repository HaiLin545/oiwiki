class Solution {
public:
    int work(int n, int dir){
        if(n==1)return 1;
        return work(n/2, 1-dir) * 2 - dir * (1- n & 1);
    }
    int lastRemaining(int n) {
        return work(n, 0);
    }
};