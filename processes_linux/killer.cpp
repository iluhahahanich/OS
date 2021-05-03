#include <functional>
#include <cstring>
#include <csignal>
#include <vector>

#include "help_functions.h"

void KillProcsByName(char **names, int n){
    ForEachProc([&](char *name, int pid){
        for (int i = 0; i < n; ++i) {
            if (strcmp(name, names[i]) == 0){
                kill(pid, SIGKILL);
                break;
            }
        }
        return false;
    });
}

void ProcessArgs(int argc, char **argv){
    if (argc >= 3) {
        if (strcmp(argv[1], "--id") == 0) {
            if (int id = ToInt(argv[2])) {
                kill(id, SIGKILL);
            }
        } else if (strcmp(argv[1], "--name") == 0) {
            char *procName = new char[BUFFER_SIZE];
            strcpy(procName, argv[2]);
            KillProcsByName(&procName, 1);
        }
    }
}

void ProcessVariable(){
    auto res = getenv("PROC_TO_KILL");
    if (!res) {
        return;
    }
    std::vector<char*> processes;
    for (int start = 0, end = 0, ok = true; ok; ++start, ++end) {
        while (res[end] && res[end] != ',') { ++end; }
        auto proc = new char[BUFFER_SIZE];
        strncpy(proc, res + start, end - start);
        proc[end - start] = '\0';
        processes.push_back(proc);
        if (!res[end]) {
            ok = false;
        }
        start = end;
    }

    KillProcsByName(processes.data(), (int)processes.size());
}

int main(int argc, char **argv) {
    ProcessArgs(argc, argv);
    ProcessVariable();
}