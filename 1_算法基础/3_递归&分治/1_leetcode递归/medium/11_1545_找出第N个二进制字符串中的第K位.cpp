//1 . 模拟 + 递归

class Solution {
public:
    int *len = nullptr;
    void work(char * res, int n){
        if(n==1){
            res[1]=0;
        }
        else {
            work(res, n-1);
            res[len[n-1]+1]=1;
            for(int i=1, j=len[n];i<j;i++,j--){
                res[j] = 1-res[i]; 
            }
        }
    }
    char findKthBit(int n, int k) {
        len = new int[n+1];
        len[1]=1;
        for(int i=2;i<=n;i++){
            len[i] = 2 * len[i-1] +1;
        }
        char * res = new char[len[n]+1];
        work(res, n);
        return '0'+ res[k];
    }
};

// 2. 递归
class Solution2 {
public:
    char findKthBit(int n, int k) {
        if(n==1)return '0';
        int mid = (1<<(n-1));
        if(k==mid)return '1';
        else if(k<mid) {
            return findKthBit(n-1, k);
        }else{
            return invert(findKthBit(n-1, mid*2 - k));
        }

    }

    char invert(char a){
        return '1' - a + '0';
    }
};
