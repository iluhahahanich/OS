#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>

#include "matrices/matrix_operations.h"
#include "matrices/rows_cols_threads_mul.h"
#include "matrices/cols_rows_threads_mul.h"
#include "matrices/block_threads_mul.h"

template <typename T>
void Check(std::vector<std::vector<T>>& res,
           std::vector<std::vector<T>>& resCur,
           const std::function<void()>& Mul){
    auto prev = std::chrono::system_clock::now();
    Mul();
    std::cout << (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - prev).count() / 1000 << "\n";
    std::cout << (resCur == res ? "equals" : "not equals") << "\n\n";
}


int main() {
    std::ifstream in("../input.txt");

    int k, n1, m1, n2, m2;
    in >> k >> n1 >> m1 >> n2 >> m2;

    std::vector<std::vector<int>> matrix1(n1, std::vector<int>(m1)), matrix2(n2, std::vector<int>(m2));
    ReadMatrix(in, matrix1);
    ReadMatrix(in, matrix2);

    std::vector<std::vector<int>> res, resCur;

    auto prev = std::chrono::system_clock::now();
    res = SimpleMul(matrix1, matrix2);
    std::cout << "[time Simple]\t" << (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - prev).count() / 1000 << "\n\n";

    Check(res, resCur, [&](){
        resCur = rows_cols::Mul(k, matrix1, matrix2);
        std::cout << "[time RowsCols]\t";
    });

    Check(res, resCur, [&](){
        resCur = cols_rows::Mul(k, matrix1, matrix2);
        std::cout << "[time ColsRows]\t";
    });

    Check(res, resCur, [&](){
        resCur = blocks::Mul(k, matrix1, matrix2);
        std::cout << "[time Blocks]\t";
    });
}
