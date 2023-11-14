//
// Created by Juno Strunk on 4/19/21.
//

#include "../Headers/Random.h"

//redeclare variable
std::mt19937 Random::random(time(0));

int Random::Int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(random);
}

int Random::Float(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(random);
}
