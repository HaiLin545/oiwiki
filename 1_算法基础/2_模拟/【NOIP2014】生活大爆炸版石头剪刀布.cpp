#include<iostream>
using namespace std;

const int N = 200;
int n, a, b, A[N], B[N];

int RockPaperScissors[5][5] = {
    {0,-1,1,1,-1},
    {1,0,-1,1,-1},
    {-1,1,0,-1,1},
    {-1,-1,1,0,1},
    {1,1,-1,-1,0},
};

int main() {
    cin >> n >> a >> b;
    for (int i = 0;i < a;i++) {
        cin >> A[i];
    }
    for (int i = 0;i < b;i++) {
        cin >> B[i];
    }
    int p = 0;
    int q = 0;
    int res = 0;
    int sa = 0, sb = 0;
    while (n--) {
        res = RockPaperScissors[A[p]][B[q]];
        if (res == 1) {
            sa++;
        }
        else if (res == -1) {
            sb++;
        }
        p++;
        q++;
        if (p == a) {
            p = 0;
        }
        if (q == b) {
            q = 0;
        }
    }
    cout << sa << " " << sb << endl;


    return 0;
}