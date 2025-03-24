#include<iostream>
#include<cmath>
#include<string>
using namespace std;

#define MOD 20150523
#define MAX 1000001

string S, E;
int result,last;

//0,1,2,4,5,7,8 -> 7^3 = 343    1~1000 : 1000-343 = 657
// 10^n - 7^n
// 3015 -> (1~999) + (1000~1999) + ((2000~2999) + (3000~E) = 1986, 첫번째 숫자가 369면 예외처리
// result = E까지의 횟수 - S까지의 횟수


// S가 369에 포함될 경우 -1

int dp(string clap, int digit) {
    if (digit <= 0) {
        for (int i = 1; i <= last; i++) {
            if (i == 3 || i == 6 || i == 9) result++;
        }
        return 0;
    }

    for (int j = 0; j < clap[0] - '0'; j++) { //첫번째 숫자
        if (j != 3 && j != 6 && j != 9) {
            result += pow(10, digit) - pow(7, digit);
        }
        else result += pow(10, digit);
    }

    if (clap[0] == '3' || clap[0] == '6' || clap[0] == '9') //첫번째 숫자가 369면 어차피 다 칠껄
    {
        clap = clap.substr(1);
        result += stoi(clap);
        return 0;
    }

    clap = clap.substr(1);
    dp(clap, digit - 1);
}

int main(void)
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> S >> E;

    int Edigit = E.length() - 1;
    int Sdigit = S.length() - 1;
    last = E[Edigit] - '0';

    dp(E, Edigit);
    int clap = result;

    result = 0;
    last = S[Sdigit] - '0';
    dp(S, Sdigit);
    int unclap = result;


    int Sfirst = S[Sdigit] - '0';
    cout << Sfirst << endl;
    if (Sfirst == 3 || Sfirst == 6 || Sfirst == 9) { //S가 포함되면 -1
        unclap--;
    }

    cout << clap << endl;
    cout << unclap << endl;

    cout << clap - unclap;
}