// randomizer.hpp
// easy way to select a random value
#pragma once

#include <cstdlib>
#include <ctime>

namespace rndm
    {
        inline void initRandom()
            {
                std::srand(std::time(NULL));
            };

        inline float random(float min, float max) { return (rand() / float(RAND_MAX))*(max - min) + min; }
        inline int random(int min, int max) { return rand() % (max - min + 1) + min; }
    }