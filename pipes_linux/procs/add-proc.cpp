#include <iostream>

#include "help-funcs.h"

int main(){
    ForAllNums([](int num){
        std::cout << num + 18 << " ";
    });
    std::cout << "\n";
}
