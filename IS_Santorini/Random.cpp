
#include "pch.h"

#include "Utility.hpp"

#include <random>

namespace etf_santorini_bj150087d {

    namespace {
    
        std::default_random_engine _rng;

    }

    void Randomize() {
    
        _rng.seed(std::random_device()());

    }

    void Randomize(UINT seed) {
    
        _rng.seed(seed);

    }

    UINT Random(UINT max_val) {
    
        std::uniform_int_distribution<UINT> distribution(0, max_val);

        return distribution(_rng);

    }

}