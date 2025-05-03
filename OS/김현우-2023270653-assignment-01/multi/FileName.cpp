// by rlagu <2025-03-12> 구구단 출력하기

#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

int main() {

    cout << "숫자를 입력하세요:";
    int num;                 
    cin >> num;

    const int BLOCK_SIZE = 5;   
    int blockStart = 0;         
    int blockEnd = 0;           
    int i = 0;                  
    int j = 0;

    for (blockStart = 1; blockStart <= num; blockStart += BLOCK_SIZE) {
        blockEnd = min(blockStart + BLOCK_SIZE - 1, num);

        for (i = 1; i <= blockEnd; i++) {
            for (j = blockStart; j <= blockEnd; j++) {
        
                if (j < i)
                    cout << setw(15) << " "; //공백 출력 (출력 폭 조절)
                else
                    cout << "|" << setw(2) << j << " * " << setw(2) << i << " = " << setw(3) << (i * j) << " ";
            }
            cout << "\n";  // 각 행이 끝날 때마다 줄바꿈
        }
        cout << "\n";  // 각 블록이 끝날 때마다 블록 간의 구분을 위해 빈 줄 출력
    }

    return 0;
}
