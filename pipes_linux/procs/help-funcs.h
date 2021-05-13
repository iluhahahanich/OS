#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <sstream>

void ForAllNums(const std::function<void(int)>& Do){
    std::string line;
    getline(std::cin, line, '\n');
    std::stringstream ss {line};
    int num;
    while(ss >> num){
        Do(num);
    }
}