#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <cstring>

void ConnectProc(const char* name, int op[2]){
    int pid = fork();
    if (pid == 0){
        dup2(op[0], STDIN_FILENO);
        dup2(op[1], STDOUT_FILENO);
        execlp(name, name, nullptr);
        exit(1);
    }
    waitpid(pid, nullptr, 0);
}

int main() {
    int fd[2];
    pipe(fd);
    const char* input = "1 2 3\n";
    write(fd[1], input, strlen(input));

    ConnectProc("./pow-proc", fd);
    ConnectProc("./mul-proc", fd);
    ConnectProc("./add-proc", fd);
    ConnectProc("./sum-proc", fd);

    const int BUFF_SIZE = 13;
    char *res = new char[BUFF_SIZE];
    read(fd[0], res, BUFF_SIZE);
    std::cout << res;
}

// 1 2 3 ->[^3]->
// 1 8 27 ->[*7]->
// 7 56 189 ->[+18]->
// 25 74 207 ->[sum]->
// 306