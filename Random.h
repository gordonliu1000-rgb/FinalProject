#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED
#include <random>


class Random {
public:
    static std::mt19937 rng;

    static int range(int l, int r){
        std::uniform_int_distribution<int> dist(l, r);
        return dist(rng);
    }
    static float range(float l, float r){
        std::uniform_real_distribution<float> dist(l, r);
        return dist(rng);
    }
};



#endif