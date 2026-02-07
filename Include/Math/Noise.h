#pragma once
#include <cstdint>
#include <vector>

namespace math {
    class Noise {
    public:
        Noise();
        Noise(uint64_t seed);

        //2d and 3d Simplex Noise using glm functions - temporary only.
        static float simplexNoise(float x, float y);
        static float simplexNoise(float x, float y, float z);


        //2d simplex noise using our implementation
        float generateNoise2d(float x, float y);

    private:

        double dot(const int16_t g[], double x, double y);

        std::vector<uint32_t> m_permutations;
        uint32_t m_perms[512];
        uint32_t m_seed;

        static constexpr int16_t grad3[12][3] = {
            { 1,  1,  0}, { -1,  1,  0}, { 1, -1,  0}, { -1, -1,  0},
            { 1,  0,  1}, { -1,  0,  1}, { 1,  0, -1}, { -1,  0, -1},
            { 0,  1,  1}, { 0, -1,  1}, { 0,  1, -1}, { 0, -1, -1}
        };
    };
}