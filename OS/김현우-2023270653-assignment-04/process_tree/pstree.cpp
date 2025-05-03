/*******************************************************************************
filename : pstree.cpp
author   : 김현우
date     : 2025-04-19
repository:
organizations:

description: Display the following information of all processes
             print the Number of Running processes
             - Process ID
             - Process Name
             - Parent Process ID
             Print them on the screen in a tree structure.

             PID가 0인 process는 System Process입니다.
             Parent processs가 존재하지 않는 경우는 0번 프로세스가 부모 프로세스입니다.
             

References: CreateToolhelp32Snapshot()
            Process32FirstA()
            Process32NextA()

@This file was written by rlagudn04
*******************************************************************************/


#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct ProcessInfo {
    DWORD pid;
    DWORD ppid;
    string name;
};

map<DWORD, vector<DWORD>> childrenMap;
map<DWORD, ProcessInfo> processMap;
int processCount = 0;

void printTree(DWORD pid, int depth = 0) {
    if (processMap.find(pid) == processMap.end()) return;

    for (int i = 0; i < depth; ++i) printf("    ");

    const ProcessInfo& proc = processMap[pid];
    printf("|- [%05d] %s (Parent: %05d)\n",
           proc.pid,
           proc.name.c_str(),
           proc.ppid);

    for (DWORD childPID : childrenMap[pid]) {
        if (childPID == pid) continue; // Prevent self-recursion
        printTree(childPID, depth + 1);
    }
}

void CollectProcessInfo() {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        printf("Failed to take snapshot.\n");
        return;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnap, &pe)) {
        printf("Process32First failed.\n");
        CloseHandle(hSnap);
        return;
    }

    do {
        ProcessInfo p;
        p.pid = pe.th32ProcessID;
        p.ppid = pe.th32ParentProcessID;
        p.name = (p.pid == 0) ? "[System Process]" : string(pe.szExeFile);

        processMap[p.pid] = p;
        processCount++;
    } while (Process32Next(hSnap, &pe));

    CloseHandle(hSnap);

    // 연결되지 않은 프로세스를 0번의 자식으로 연결 
    for (map<DWORD, ProcessInfo>::iterator it = processMap.begin(); it != processMap.end(); ++it) {
        DWORD pid = it->first;
        DWORD ppid = it->second.ppid;
        if (pid == 0) continue;
        if (processMap.find(ppid) == processMap.end()) {
            childrenMap[0].push_back(pid);
        } else {
            childrenMap[ppid].push_back(pid);
        }
    }
}

int main() {
    CollectProcessInfo();

    printf("==============================================\n");
    printf("Number of Running processes: %d\n", processCount);
    printf("[Process Tree at Snapshot]\n");
    printf("==============================================\n\n");

    printTree(0);

    return 0;
}
