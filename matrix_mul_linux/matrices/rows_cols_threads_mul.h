#pragma once

#include <queue>
#include <vector>

#include <pthread.h>

#include "matrix_operations.h"

namespace rows_cols {

    class MulSeq {
        int i = 0, j = 0, m = 0;
    public:
        explicit MulSeq(int m) : m(m) {}

        void Set(std::vector<int>& v){
            v = {i, j};
            Update();
        }

        bool HasElem() const {
            return i != m;
        }

        void Update() {
            j += 1;
            if (j == m) {
                j = 0;
                i += 1;
            }
        }
    };


    pthread_mutex_t mtxQueue = PTHREAD_MUTEX_INITIALIZER;

    template <typename T>
    struct Params {
        const std::vector<std::vector<T>> *m1, *m2;
        std::vector<std::vector<T>> *ans;
        MulSeq *mulSeq;
        const std::vector<int> *bounds1, *bounds2;
    };

    template <typename T>
    void *ThreadProc(void *params){
        auto par = (Params<T>*) params;
        auto &m1 = *par->m1, &m2 = *par->m2;
        auto &ans = *par->ans;
        auto &mulSeq = *par->mulSeq;
        auto &bounds1 = *par->bounds1, &bounds2 = *par->bounds2;

        pthread_mutex_lock(&mtxQueue);
        while (mulSeq.HasElem()){
            std::vector<int> cur;
            mulSeq.Set(cur);
            pthread_mutex_unlock(&mtxQueue);

            auto res = SimpleMul(SubMatrix(m1, bounds1[cur[0]], bounds1[cur[0] + 1], 0, m1[0].size()),
                                 SubMatrix(m2, 0, m2.size(), bounds2[cur[1]], bounds2[cur[1] + 1]));

            Assign(ans, bounds1[cur[0]], bounds1[cur[0] + 1], bounds2[cur[1]], bounds2[cur[1] + 1], res);

            pthread_mutex_lock(&mtxQueue);
            
        }
        pthread_mutex_unlock(&mtxQueue);

        return nullptr;
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

    template <typename T>
    std::vector<std::vector<T>> Mul(int k, const std::vector<std::vector<T>>& m1, const std::vector<std::vector<T>>& m2){

        auto mulSeq = MulSeq(k);

        auto bounds1 = Bounds(k, m1.size()),
             bounds2 = Bounds(k, m2[0].size());


        auto threads = new pthread_t[k];
        std::vector<std::vector<T>> ans(m1.size(), std::vector<T>(m2[0].size()));
        auto params = new Params<T>{&m1, &m2, &ans, &mulSeq, &bounds1, &bounds2};

        for (int i = 0; i < k; ++i) {
            pthread_create(&threads[i], NULL, ThreadProc<T>, params);
        }

        for (int i = 0; i < k; ++i){
            pthread_join(threads[i], nullptr);
        }
        delete[] threads;


        return ans;
    }
}
