#pragma once

#include <pthread.h>

#include <vector>

#include "matrix_operations.h"

namespace cols_rows {

    pthread_mutex_t mtxMatrix = PTHREAD_MUTEX_INITIALIZER;

    template <typename T>
    struct Params {
        const std::vector<std::vector<T>> *m1, *m2;
        std::vector<std::vector<T>> *ans;
        int begin, end;
    };

    template <typename T>
    void *ThreadProc(void *params){
        auto par = (Params<T>*) params;
        auto &m1 = *par->m1, &m2 = *par->m2;
        auto &ans = *par->ans;
        int begin = par->begin, end = par->end;

        auto res = SimpleMul(SubMatrix(m1, 0, m1.size(), begin, end),
                             SubMatrix(m2, begin, end, 0, m2[0].size()));

        pthread_mutex_lock(&mtxMatrix);

        ans = Sum(ans, res);

        pthread_mutex_unlock(&mtxMatrix);

        return nullptr;
    }

    template <typename T>
    std::vector<std::vector<T>> Mul(int k, std::vector<std::vector<T>>& m1, std::vector<std::vector<T>>& m2){

        std::vector<int> elemNum(k, m1[0].size() / k);
        for (int i = 0; i < m1[0].size() % k; ++i) {
            elemNum[i] += 1;
        }

        auto threads = new pthread_t[k];
        std::vector<std::vector<T>> ans(m1.size(), std::vector<T>(m2[0].size()));

        for (int i = 0, cur = 0; i < k; ++i) {
            auto params = new Params<T>{&m1, &m2, &ans, cur, cur + elemNum[i]};
            pthread_create(&threads[i], NULL, ThreadProc<T>, params);
            cur += elemNum[i];
        }

        for (int i = 0; i < k; ++i){
            pthread_join(threads[i], nullptr);
        }
        delete[] threads;


        return ans;
    }
}
