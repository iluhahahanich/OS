#include <cstdio>
#include <functional>
#include <unistd.h>
#include <sys/wait.h>

#include "help_functions.h"

bool IsRunning(const char *proc){
    bool running = false;
    ForEachProc([&](char *name, int pid){
        if (strcmp(proc, name) == 0){
            running = true;
            return true;
        }
        return false;
    });
    return running;
}

void Check(const char *name){
    if (IsRunning(name)){
        printf("proc %s is running\n", name);
    } else {
        printf("proc %s is NOT running\n", name);
    }
}

int StartApp(const char* name){
    int pid = fork();
    if (pid == 0) {
        execlp(name, name, nullptr);
        exit(1);
    }
    return pid;
}

bool Check(int id){
    if (kill(id, SIGCONT) == 0){
        printf("proc with id %d is running\n", id);
    } else {
        printf("proc with id %d is NOT running\n", id);
    }
}

int main() {
    setenv("PROC_TO_KILL", "firefox,baobab", true);

    int pid1 = StartApp("baobab");
    int pid2 = StartApp("gedit");
    int pid3 = StartApp("calc");

    sleep(2);

    Check("gedit");
    Check("baobab");
    Check(pid3);
    printf("\n");

    int pid = fork();
    if (pid == 0){
        execlp("./killer", "./killer", "--name", "gedit", nullptr);
    } else {
        waitpid(pid, nullptr, 0);
    }

    pid = fork();
    if (pid == 0){
        execlp("./killer", "./killer", "--id", std::to_string(pid3).c_str(), nullptr);
    } else {
        waitpid(pid, nullptr, 0);
    }
    sleep(1);

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
    waitpid(pid3, nullptr, 0);

    Check("gedit");
    Check("baobab");
    Check(pid3);

//    PrintProcs();

    unsetenv("PROC_TO_KILL");
}