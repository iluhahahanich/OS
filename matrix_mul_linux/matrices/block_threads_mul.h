#pragma once

#include <pthread.h>

#include <vector>

#include "matrix_operations.h"

namespace blocks {

	class MulSeq {
        int i = 0, j = 0, k = 0, m = 0;
    public:
        explicit MulSeq(int m) : m(m) {}

        void Set(std::vector<int>& v){
            v = {i, j, k};
            Update();
        }

        bool HasElem() const {
            return i != m;
        }

        void Update(){
            k += 1;
            if (k == m){
                k = 0;
                j += 1;
                if (j == m){
                    j = 0;
                    i += 1;
                }
            }
        }
    };

    pthread_mutex_t mtxQueue = PTHREAD_MUTEX_INITIALIZER;
    std::vector<std::vector<pthread_mutex_t>> mtxBlock;

    template <typename T>
    struct Params {
        const std::vector<std::vector<T>> *m1, *m2;
        std::vector<std::vector<T>> *ans;
        MulSeq *mulSeq;
        const std::vector<int> *bounds1, *bounds2, *bounds3;
    };

    template <typename T>
    void * ThreadProc(void *params) {
        auto par = (Params<T> *) params;
        auto &m1 = *par->m1, &m2 = *par->m2;
        auto &ans = *par->ans;
         auto &bounds1 = *par->bounds1, &bounds2 = *par->bounds2, &bounds3 = *par->bounds3;
        auto &mulSeq = *par->mulSeq;
        
        pthread_mutex_lock(&mtxQueue);
        while (mulSeq.HasElem()){
            std::vector<int> cur;
            mulSeq.Set(cur);
            pthread_mutex_unlock(&mtxQueue);

            auto res = SimpleMul(
                    SubMatrix(m1, bounds1[cur[0]], bounds1[cur[0] + 1], bounds2[cur[1]], bounds2[cur[1] + 1]),
                    SubMatrix(m2, bounds2[cur[1]], bounds2[cur[1] + 1], bounds3[cur[2]], bounds3[cur[2] + 1]));
            pthread_mutex_lock(&mtxBlock[cur[0]][cur[2]]);

			Assign(ans,
                   bounds1[cur[0]], bounds1[cur[0] + 1], bounds3[cur[2]], bounds3[cur[2] + 1],
                   Sum(res,
                       SubMatrix(ans, bounds1[cur[0]], bounds1[cur[0] + 1], bounds3[cur[2]], bounds3[cur[2] + 1])));

            pthread_mutex_unlock(&mtxBlock[cur[0]][cur[2]]);
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

    template<typename T>
    std::vector<std::vector<T>> Mul(int k, const std::vector<std::vector<T>> &m1, const std::vector<std::vector<T>> &m2) {

        mtxBlock.resize(k, std::vector<pthread_mutex_t>(k, PTHREAD_MUTEX_INITIALIZER));

        auto mulSeq = MulSeq(k);
		auto bounds1 = Bounds(k, m1.size()),
             bounds2 = Bounds(k, m1[0].size()),
             bounds3 = Bounds(k, m2[0].size());

        std::vector<std::vector<T>> ans(m1.size(), std::vector<T>(m2[0].size()));
        auto params = new Params<T>{&m1, &m2, &ans, &mulSeq, &bounds1, &bounds2, &bounds3};

        auto threads = new pthread_t[k];
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