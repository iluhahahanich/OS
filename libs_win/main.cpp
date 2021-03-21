#include <iostream>
#include "number.h"
#include "vector.h"

int main() {
    Number a(7), b = CreateNumber(13);
    std::cout << "(nums)\t" << a << ", " << b << '\n';
    std::cout << "(+)\t" << a + b << '\n';
    std::cout << "(-)\t" << a - b << '\n';
    std::cout << "(*)\t" << a * b << '\n';
    std::cout << "(/)\t" << a / b << '\n';
    std::cout << '\n';

    Vector x(ONE, b), y(Number(2), a), z = x + y;
    std::cout << "(vectors)>\t" << x << ", " << y << '\n';
    std::cout << "(sum)>\t\t" << z << '\n';
    std::cout << "(polar)>\t(" << z.Fi() << ", " << z.Ro() << ")\n";
}
