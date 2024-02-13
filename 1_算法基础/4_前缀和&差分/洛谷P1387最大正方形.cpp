#include <iostream>

using namespace std;

int a[101][101]{0};
int s[101][101]{0};

int main()
{

    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cin >> a[i][j];
            s[i][j] = s[i][j - 1] + s[i - 1][j] + a[i][j] - s[i - 1][j - 1];
        }
    }

    int max_len = 0;

    for (int i = 1; i <= n; i++)
    {

        for (int j = 1; j <= m; j++)
        {

            int k = max(max_len, 1);
            while (i - k + 1 > 0 && j - k + 1 > 0)
            {
                int local_sum = s[i][j] - s[i - k][j] - s[i][j - k] + s[i - k][j - k];
                if (local_sum == k * k)
                {
                    max_len = max(max_len, k);
                    k++;
                }
                else
                {
                    break;
                }
            }
        }
    }

    cout << max_len << endl;

    return 0;
}