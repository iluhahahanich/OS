#pragma once

#include <dirent.h>
#include <cstdio>
#include <cctype>
#include <functional>
#include <cstring>
#include <csignal>

const int BUFFER_SIZE = 260;

int ToInt(const char *str){
    int ans = 0;
    for (int i = 0; str[i]; ++i) {
        if (str[i] > '9' || str[i] < '0') {
            auto msg = new char[BUFFER_SIZE];
            sprintf(msg, "format number exception (in string '%s')", str);
            perror(msg);
            return 0;
        }
        ans = ans * 10 + str[i] - '0';
    }
    return ans;
}


bool IsNum(const struct dirent *entry) {
    for (const char *p = entry->d_name; *p; p++) {
        if (!isdigit(*p)) {
            return false;
        }
    }
    return true;
}

int ForEachProc(const std::function<bool(char*, int)>& Do){
    FILE *file;
    struct dirent *entry;
    char path[BUFFER_SIZE];
    int pid;

    DIR *dir = opendir("/proc");
    bool made = false;
    while ((entry = readdir(dir)) && !made) {
        if (!IsNum(entry)) {
            continue;
        }

        sprintf(path, "/proc/%s/stat", entry->d_name);
        file = fopen(path, "r");

        if (!file) {
            perror(path);
            continue;
        }

        fscanf(file, "%d %s", &pid, &path);
        auto name = new char[BUFFER_SIZE];
        strcpy(name, path + 1);
        name[strlen(path) - 2] = '\0';

        if (Do(name, pid)) {
            made = true;
        }

        fclose(file);
    }
    closedir(dir);
    return 0;
}


void PrintProcs(){
    ForEachProc([](char* name, int pid){
        printf("%5d %-s\n", pid, name);
        return false;
    });
}
