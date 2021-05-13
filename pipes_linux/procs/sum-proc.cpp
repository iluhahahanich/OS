#include <iostream>

#include "help-funcs.h"

int main(){
    int sum = 0;
    ForAllNums([&](int num){
        sum += num;
    });
    std::cout << sum << '\n';
}
