#include <iostream>
#include <fstream>
#include <functional>

#include "matrices/matrix_operations.h"
#include "matrices/rows_cols_threads_mul.h"
#include "matrices/cols_rows_threads_mul.h"
#include "matrices/block_threads_mul.h"

template <typename T>
void Check(std::vector<std::vector<T>>& resPrev,
           std::vector<std::vector<T>>& resCur,
           const std::function<void()>& Mul){
    long prev = clock();
    Mul();
    std::cout << (clock() - prev) / 1000.0 << "\n";
    std::cout << (resCur == resPrev ? "equals" : "not equals") << "\n\n";
    resPrev = resCur;
}


int main() {
    std::ifstream in("../input.txt");

    int k, n1, m1, n2, m2;
    in >> k >> n1 >> m1 >> n2 >> m2;

    std::vector<std::vector<int>> matrix1(n1, std::vector<int>(m1)), matrix2(n2, std::vector<int>(m2));
    ReadMatrix(in, matrix1);
    ReadMatrix(in, matrix2);

    std::vector<std::vector<int>> resPrev, resCur;

    long prev = clock();
    resPrev = SimpleMul(matrix1, matrix2);
    std::cout << "[time Simple]\t" << (clock() - prev) / 1000.0 << "\n\n";

    Check(resPrev, resCur, [&](){
        resCur = rows_cols::Mul(k, matrix1, matrix2);
        std::cout << "[time RowsCols]\t";
    });

    Check(resPrev, resCur, [&](){
        resCur = cols_rows::Mul(k, matrix1, matrix2);
        std::cout << "[time ColsRows]\t";
    });

    Check(resPrev, resCur, [&](){
        resCur = blocks::Mul(k, matrix1, matrix2);
        std::cout << "[time Blocks]\t";
    });
}
