// by rlagu <2025-03-12> ������ ����ϱ�

#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

int main() {

    cout << "���ڸ� �Է��ϼ���:";
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
                    cout << setw(15) << " "; //���� ��� (��� �� ����)
                else
                    cout << "|" << setw(2) << j << " * " << setw(2) << i << " = " << setw(3) << (i * j) << " ";
            }
            cout << "\n";  // �� ���� ���� ������ �ٹٲ�
        }
        cout << "\n";  // �� ����� ���� ������ ��� ���� ������ ���� �� �� ���
    }

    return 0;
}
