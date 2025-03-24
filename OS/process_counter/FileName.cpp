#include <iostream>
#include <Windows.h>
#include <Psapi.h>
using namespace std;

/*
by Kim, Hyunwoo  
2025-03-21
process counter: �� �ʸ��� ���� �ð��� ���� ���� ���μ��� ������ ���
*/

// ���� ���� ���� ���μ��� ������ ��ȯ
int getProcessCount() {
    DWORD processes[1024], needed;


    if (!EnumProcesses(processes, sizeof(processes), &needed)) {
        return 0;
    }

    return needed / sizeof(DWORD);
}

// ���� �ð��� ���
void printCurrentTime() {
    SYSTEMTIME time;
    GetLocalTime(&time); 

    cout << time.wYear << "-"
        << time.wMonth << "-"
        << time.wDay << " "
        << time.wHour << ":"
        << time.wMinute << ":"
        << time.wSecond << " : ";
}

int main() {
    while (true) {
        printCurrentTime(); 

        int runningProcesses = getProcessCount();
        cout << runningProcesses << endl;

   
        Sleep(1000);
    }

    return 0;
}
