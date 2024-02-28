class Solution {
public:
    int multiply(int A, int B) {
        if(B==1)return A;
        if(B==0)return 0;
        return (A<<1) + multiply(A, B-2);
    }
};