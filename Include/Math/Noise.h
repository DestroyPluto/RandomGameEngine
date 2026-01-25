#pragma once

namespace math {
    class Noise {
    public:
        //2d Simplex Noise
        static float simplexNoise(float x, float y);
        static float simplexNoise(float x, float y, float z);
    };
}