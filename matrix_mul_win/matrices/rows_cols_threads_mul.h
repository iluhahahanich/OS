#pragma once

#include <Windows.h>

#include <queue>
#include <vector>

#include "matrix_operations.h"

namespace rows_cols {

    HANDLE mtxQueue;

    template <typename T>
    struct Params {
        const std::vector<std::vector<T>> *m1, *m2;
        std::vector<std::vector<T>> *ans;
        std::queue<std::vector<int>> *mulSeq;
        const std::vector<int> *bounds;
    };

    template <typename T>
    DWORD WINAPI ThreadProc(void *params){
        auto par = (Params<T>*) params;
        auto &m1 = *par->m1, &m2 = *par->m2;
        auto &ans = *par->ans;
        auto &bounds = *par->bounds;
        auto &mulSeq = *par->mulSeq;

        WaitForSingleObject(mtxQueue, INFINITE);
        while (!mulSeq.empty()){
            auto cur = mulSeq.front();
            mulSeq.pop();
            ReleaseMutex(mtxQueue);

            auto res = SimpleMul(SubMatrix(m1, bounds[cur[0]], bounds[cur[0] + 1], 0, m1[0].size()),
                                 SubMatrix(m2, 0, m2.size(), bounds[cur[1]], bounds[cur[1] + 1]));

            Assign(ans, bounds[cur[0]], bounds[cur[0] + 1], bounds[cur[1]], bounds[cur[1] + 1], res);

            WaitForSingleObject(mtxQueue, INFINITE);
        }
        ReleaseMutex(mtxQueue);

        return 0;
    }

    std::vector<int> Bounds(int k, int m) {
        std::vector<int> elemNum(k, m / k);
        for (int i = 0; i < m % k; ++i) {
            elemNum[i] += 1;
        }

        std::vector<int> bounds(k + 1);
        for (int i = 1; i < k + 1; ++i) {
            bounds[i] = bounds[i - 1] + elemNum[i - 1];
        }

        return bounds;
    }

    std::queue<std::vector<int>> MulSeq(int k){
        std::queue<std::vector<int>> mulSeq;
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                mulSeq.push({i, j});
            }
        }
        return mulSeq;
    }

    template <typename T>
    std::vector<std::vector<T>> Mul(int k, const std::vector<std::vector<T>>& m1, const std::vector<std::vector<T>>& m2){

        mtxQueue = CreateMutex(nullptr, false, nullptr);
        auto mulSeq = MulSeq(k);

        auto bounds = Bounds(k, m1.size());

        auto threads = new HANDLE[k];
        std::vector<std::vector<T>> ans(m1.size(), std::vector<T>(m2[0].size()));
        auto params = new Params<T>{&m1, &m2, &ans, &mulSeq, &bounds};

        for (int i = 0; i < k; ++i) {
            threads[i] = CreateThread(nullptr, 0, ThreadProc<T>, params, 0, nullptr);
        }

        WaitForMultipleObjects(k, threads, TRUE, INFINITE);

        for (int i = 0; i < k; ++i) {
            CloseHandle(threads[i]);
        }
        delete[] threads;

        CloseHandle(mtxQueue);

        return ans;
    }
}
