#include<iostream>
using namespace std;


const int p = 6;
const int q = 5;
int a[q][p] = { 0 };
int t[q][p] = { 0 };
int res[q][p] = { 0 };
int dir[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
int xx, yy;

void click(int x, int y) {
    // cout << "click " << x << "," << y << endl;
    res[x][y] = 1;
    t[x][y] = 1 - t[x][y];
    for (int i = 0;i < 4;i++) {
        xx = x + dir[i][0];
        yy = y + dir[i][1];
        if (xx >= 0 && xx < q && yy >= 0 && yy < p) {
            t[xx][yy] = 1 - t[xx][yy];
        }
    }
}

void init() {
    for (int i = 0;i < q;i++) {
        for (int j = 0;j < p;j++) {
            t[i][j] = a[i][j];
            res[i][j] = 0;
        }
    }
}

void print() {
    cout << "clicked" << endl;
    for (int i = 0;i < q;i++) {
        for (int j = 0;j < p;j++) {
            cout << res[i][j] << " ";
        }
        cout << endl;
    }
    cout << "result" << endl;
    for (int i = 0;i < q;i++) {
        for (int j = 0;j < p;j++) {
            cout << t[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {

    for (int i = 0;i < q;i++) {
        for (int j = 0;j < p;j++) {
            cin >> a[i][j];
        }
    }

    // cin >> p;
    int n = 1 << p;
    int b[p];

    for (int i = 0;i < n;i++) {
        int x = i;
        init();
        // 枚举第一行的开关灯情况
        for (int j = 0;j < p;j++) {
            b[j] = x & 1;
            x >>= 1;
            if (b[j]) {
                click(0, j);
            }
        }


        // 从第二行开始，若上一行同一列的位置开灯，则点击该位置
        for (int k = 1;k < q;k++) {
            for (int j = 0;j < p;j++) {
                if (t[k - 1][j] == 1) {
                    click(k, j);
                }
            }

        }


        // 最后一行，如果全部灯已经关闭，则该方案可行
        int flag = true;
        for (int j = 0;j < p;j++) {
            if (t[q - 1][j] == 1) {
                flag = false;
                break;
            }
        }

        // 若可行，输出结果并退出程序
        if (flag) {

            for (int k = 0;k < q;k++) {
                for (int j = 0;j < p;j++) {
                    cout << res[k][j] << " ";
                }
                cout << endl;
            }
            // cout << endl;

            // for (int i = 0;i < q;i++) {
            //     for (int j = 0;j < p;j++) {
            //         cout << t[i][j] << " ";
            //     }
            //     cout << endl;
            // }
            break;
        }
    }

    return 0;
}