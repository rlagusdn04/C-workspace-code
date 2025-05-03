#include <iostream>
#include <Windows.h>
#include <Psapi.h>
using namespace std;

/*
by Kim, Hyunwoo  
2025-03-21
process counter: 매 초마다 현재 시간과 실행 중인 프로세스 개수를 출력
*/

// 현재 실행 중인 프로세스 개수를 반환
int getProcessCount() {
    DWORD processes[1024], needed;


    if (!EnumProcesses(processes, sizeof(processes), &needed)) {
        return 0;
    }

    return needed / sizeof(DWORD);
}

// 현재 시간을 출력
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
