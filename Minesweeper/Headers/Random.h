//
// Created by Juno Strunk on 4/19/21.
//

#pragma once
#include <random>

class Random
{
    static std::mt19937 random;
public:
    //one and only one of this function
    static int Int(int min, int max);
    static int Float(float min, float max);
};
