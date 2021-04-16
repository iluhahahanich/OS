#pragma once

#include <Windows.h>

#include <vector>

#include "matrix_operations.h"

namespace cols_rows {

    HANDLE mtxMatrix;

    template <typename T>
    struct Params {
        const std::vector<std::vector<T>> *m1, *m2;
        std::vector<std::vector<T>> *ans;
        int begin, end;
    };

    template <typename T>
    DWORD WINAPI ThreadProc(void *params){
        auto par = (Params<T>*) params;
        auto &m1 = *par->m1, &m2 = *par->m2;
        auto &ans = *par->ans;
        int begin = par->begin, end = par->end;

        auto res = SimpleMul(SubMatrix(m1, 0, m1.size(), begin, end),
                             SubMatrix(m2, begin, end, 0, m2[0].size()));

        WaitForSingleObject(mtxMatrix, INFINITE);

        ans = Sum(ans, res);

        ReleaseMutex(mtxMatrix);

        return 0;
    }

    template <typename T>
    std::vector<std::vector<T>> Mul(int k, std::vector<std::vector<T>>& m1, std::vector<std::vector<T>>& m2){
        mtxMatrix = CreateMutex(nullptr, false, nullptr);

        std::vector<int> elemNum(k, m1[0].size() / k);
        for (int i = 0; i < m1[0].size() % k; ++i) {
            elemNum[i] += 1;
        }

        auto threads = new HANDLE[k];
        std::vector<std::vector<T>> ans(m1.size(), std::vector<T>(m2[0].size()));

        for (int i = 0, cur = 0; i < k; ++i) {
            auto params = new Params<T>{&m1, &m2, &ans, cur, cur + elemNum[i]};
            threads[i] = CreateThread(nullptr, 0, ThreadProc<T>, params, 0, nullptr);
            cur += elemNum[i];
        }

        WaitForMultipleObjects(k, threads, TRUE, INFINITE);

        for (int i = 0; i < k; ++i) {
            CloseHandle(threads[i]);
        }
        delete[] threads;

        CloseHandle(mtxMatrix);

        return ans;
    }
}
