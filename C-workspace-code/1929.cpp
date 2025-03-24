#include<iostream>
#include<cmath>
using namespace std;

#define MAX 10000
int arr[1000000];

int main(void)
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int M, N;
    cin >> N >> M;
    
    for (int i = 2; i <= M; i++) arr[i] = i;

    for (int i = 2; i <= sqrt(M); i++) {
        if (arr[i] = 0) continue;

        for (int j = 2 * i; j <= M; j += i) {
            arr[j] = 0;
            cout << j << endl;
            if (j == 3) cout << "F";
        }
    }

    cout << arr[3];
    for (int i = N; i <= M; i++) {
        if (arr[i] != 0) cout << arr[i] << "\n";
    }
 
}