#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <string>
#include <psapi.h>

void Check(const CHAR* name){
    const int MAX_PROC_NUM = 1024;
    DWORD procId[MAX_PROC_NUM];
    DWORD procNum;

    if (!EnumProcesses(procId, sizeof(procId), &procNum)) return;
    procNum = procNum / sizeof(DWORD);

    bool running = false;
    for (int i = 0; i < procNum && !running; i++){
        HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, false, procId[i]);
        char processName[MAX_PATH];
        GetProcessImageFileName(proc, processName, sizeof(processName));
        if (strstr(processName, name)) {
            running = true;
        }
        CloseHandle(proc);
    }
    if (running) {
        printf("proc %s is running\n", name);
    } else {
        printf("proc %s is NOT running\n", name);
    }
}

void Check(int id){
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, id);
    DWORD status;
    GetExitCodeProcess(process, &status);
    if (status == STILL_ACTIVE) {
        printf("proc with ID %d is running\n", id);
    } else {
        printf("proc with ID %d is NOT running\n", id);
    }
}

int main() {
    SetEnvironmentVariable("PROC_TO_KILL", "notepad,chrome");

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    CreateProcess(nullptr, (LPSTR)"notepad", nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
    CreateProcess(nullptr, (LPSTR)"calc", nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
    CreateProcess(nullptr, (LPSTR)"mspaint", nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
    int mspaintId = pi.dwProcessId;

    for(auto& proc : {"notepad.exe", "chrome.exe", "calc.exe"}){
        Check(proc);
    }
    Check(mspaintId);
    printf("\n");

    CHAR* szCommandLine = new CHAR[]{"killer.exe --id \0"};
    strcat(szCommandLine, std::to_string(mspaintId).c_str());
    CreateProcess(nullptr, szCommandLine, nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);

    szCommandLine = new CHAR[]{"killer.exe --name calc \0"};
    CreateProcess(nullptr, szCommandLine, nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);

    for(auto& proc : {"notepad.exe", "chrome.exe", "calc.exe"}){
        Check(proc);
    }
    Check(mspaintId);

    SetEnvironmentVariable("PROC_TO_KILL", nullptr);

    system("pause");
}
