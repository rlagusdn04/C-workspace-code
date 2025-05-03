/*
File: cpu_load
Created on: 2025-03-28
Author: kim hyunwoo
*/

#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

ULONGLONG FileTimeToUInt64(const FILETIME& ft) {
    return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

double GetCpuLoad() {
    static FILETIME prevIdle = {}, prevKernel = {}, prevUser = {};
    FILETIME idle, kernel, user;

    if (!GetSystemTimes(&idle, &kernel, &user)) return -1.0;

    ULONGLONG idleDiff = FileTimeToUInt64(idle) - FileTimeToUInt64(prevIdle);
    ULONGLONG totalDiff = (FileTimeToUInt64(kernel) - FileTimeToUInt64(prevKernel))
        + (FileTimeToUInt64(user) - FileTimeToUInt64(prevUser));

    prevIdle = idle;
    prevKernel = kernel;
    prevUser = user;

    double cpuLoad;
    if (totalDiff > 0) {
        cpuLoad = (1.0 - (double)idleDiff / totalDiff) * 100.0;
    }
    else {
        cpuLoad = 0.0;
    }
    return cpuLoad;
}

void DisplayCpuLoad() {
    vector<double> cpuLoads;
    int elapsedTime = 0;

    while (true) {
        SYSTEMTIME lt;
        GetLocalTime(&lt);
        double load = GetCpuLoad();
        if (load < 0) break;

        cpuLoads.push_back(load);

        printf("%04d-%02d-%02d %02d:%02d:%02d: [CPU: %.1f%%]",
            lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, load);

        // 5초부터 시작해서 5초 단위로 증가하며 평균을 출력
        for (int cntSec = 5; cntSec <= elapsedTime; cntSec += 5) {
            double avg = 0;
            int startIdx = max(0, (int)cpuLoads.size() - cntSec);
            for (int i = startIdx; i < cpuLoads.size(); ++i) {
                avg += cpuLoads[i];
            }
            avg /= min(cntSec, (int)cpuLoads.size());

            printf(" [");
            printf("Avg %d sec: %.1f%%", cntSec, avg);
            printf("]");
        }

        cout << "\n";

        Sleep(1000);
        elapsedTime++;
    }
}

int main() {
    DisplayCpuLoad();
    return 0;
}
