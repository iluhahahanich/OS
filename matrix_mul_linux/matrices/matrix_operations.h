#pragma once

#include <vector>

template <typename T>
void PrintMatrix(const std::vector<std::vector<T>>& matrix) {
    for (auto& row : matrix){
        for (auto& elem : row){
            std::cout << elem << " ";
        }
        std::cout << '\n';
    }
}

template <typename T>
void ReadMatrix(std::ifstream& in, std::vector<std::vector<T>>& matrix) {
    for (auto& row : matrix){
        for (auto& elem : row){
            in >> elem;
        }
    }
}

template<typename T>
void Assign(std::vector<std::vector<T>>& m, int n1, int n2, int m1, int m2, const std::vector<std::vector<T>>& source) {
    for (int i = n1; i < n2; ++i) {
        for (int j = m1; j < m2; ++j) {
            m[i][j] = source[i - n1][j - m1];
        }
    }
}

template<typename T>
std::vector<std::vector<T>> Sum(const std::vector<std::vector<T>>& m1, const std::vector<std::vector<T>>& m2) {
    std::vector<std::vector<T>> ans(m1.size(), std::vector<T>(m1[0].size()));
    for (int i = 0; i < ans.size(); ++i) {
        for (int j = 0; j < ans[0].size(); ++j) {
            ans[i][j] = m1[i][j] + m2[i][j];
        }
    }
    return ans;
}

template<typename T>
std::vector<std::vector<T>> Tr(const std::vector<std::vector<T>>& m) {
    std::vector<std::vector<T>> ans(m[0].size(), std::vector<T>(m.size()));
    for (int i = 0; i < ans.size(); ++i) {
        for (int j = 0; j < ans[0].size(); ++j) {
            ans[i][j] = m[j][i];
        }
    }
    return ans;
}

template<typename T>
std::vector<std::vector<T>> SubMatrix(const std::vector<std::vector<T>>& m, int n1, int n2, int m1, int m2){
    std::vector<std::vector<T>> ans(n2 - n1, std::vector<T>(m2 - m1));
    for (int i = n1; i < n2; ++i) {
        for (int j = m1; j < m2; ++j) {
            ans[i - n1][j - m1] = m[i][j];
        }
    }
    return ans;
}

template <typename T>
std::vector<std::vector<T>> SimpleMul(const std::vector<std::vector<T>>& m1, const std::vector<std::vector<T>>& m2){
    std::vector<std::vector<T>> ans(m1.size(), std::vector<T>(m2[0].size()));
    for (int i = 0; i < ans.size(); ++i) {
        for (int j = 0; j < ans[0].size(); ++j) {
            for (int k = 0; k < m1[0].size(); ++k) {
                ans[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return ans;
}