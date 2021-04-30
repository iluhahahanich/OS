#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <Windows.h>
#include <vector>
#include <Psapi.h>

const DWORD BUFFER_SIZE = 260;

int ToInt(const char *str){
    int ans = 0;
    for (int i = 0; str[i]; ++i) {
        if (str[i] > '9' || str[i] < '0') {
            auto msg = new char[BUFFER_SIZE];
            sprintf(msg, "format number exception (in string '%s')", str);
            throw std::exception(msg);
        }
        ans = ans * 10 + str[i] - '0';
    }
    return ans;
}

void KillProcById(int id){
    HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, false, id);
    TerminateProcess(proc, 1);
    CloseHandle(proc);
}

void KillProcsByName(char **names, int n){
    const int MAX_PROC_NUM = 1024;
    DWORD procId[MAX_PROC_NUM];
    DWORD procNum;

    if (!EnumProcesses(procId, sizeof(procId), &procNum)) return;
    procNum = procNum / sizeof(DWORD);

    for (int i = 0; i < procNum; i++){
        HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, false, procId[i]);
        CHAR procName[MAX_PATH];
        GetProcessImageFileName(proc, procName, sizeof(procName));
        for (int j = 0; j < n; ++j) {
            if (strstr(procName, names[j])) {
                TerminateProcess(proc, 0);
                break;
            }
        }
        CloseHandle(proc);
    }
}


void ProcessArgs(int argc, char **argv){
    if (argc >= 3) {
        if (strcmp(argv[1], "--id") == 0) {
            int id;
            try {
                id = ToInt(argv[2]);
            } catch (const std::exception &e) {
                printf("%s", e.what());
                return;
            }
            KillProcById(id);

        } else if (strcmp(argv[1], "--name") == 0) {
            char *procName = new char[BUFFER_SIZE];
            strcpy(procName, argv[2]);
            strcat(procName, ".exe");
            KillProcsByName(&procName, 1);
        }
    }
}

void ProcessVariable(){
    auto res = new CHAR[128];
    if (!GetEnvironmentVariableA("PROC_TO_KILL", res, BUFFER_SIZE)) {
        return;
    }
    std::vector<CHAR*> processes;
    for (int start = 0, end = 0, ok = true; ok; ++start, ++end) {
        while (res[end] && res[end] != ',') { ++end; }
        auto proc = new CHAR[BUFFER_SIZE];
        strncpy(proc, res + start, end - start);
        proc[end - start] = '\0';
        strcat(proc, ".exe");
        processes.push_back(proc);
        if (!res[end]) {
            ok = false;
        }
        start = end;
    }

    KillProcsByName(processes.data(), processes.size());
}

// only one --id or --name !!
int main(int argc, char* argv[]) {
    ProcessArgs(argc, argv);
    ProcessVariable();
}
