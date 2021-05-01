#include <cstdio>
#include <functional>
#include <unistd.h>
#include <sys/wait.h>

void StartApp(const char* name){
    if (fork() == 0) {
        execlp(name, name, nullptr);
        exit(1);
    }
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

    // ubuntu default apps
    StartApp("baobab");
    StartApp("gedit");

    sleep(2);

    if (fork() == 0){
        execlp("./killer", "./killer", "--name", "gedit", nullptr);
    } else {
        wait(nullptr);
    }

    int pid = 4349;
    Check(pid);
    if (fork() == 0){
        execlp("./killer", "./killer", "--id", std::to_string(pid).c_str(), nullptr);
    } else {
        wait(nullptr);
    }
    sleep(1);
    Check(pid);

    unsetenv("PROC_TO_KILL");
}