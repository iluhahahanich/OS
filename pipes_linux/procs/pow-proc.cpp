#include <iostream>

#include "help-funcs.h"

int main(){
    ForAllNums([](int num){
        std::cout << num * num * num << " ";
    });
    std::cout << "\n";
}
