#include "Noise.h"
#include <cmath>
#include <glm/gtc/noise.hpp>
#include <algorithm>
#include <random>

using namespace math;

Noise::Noise() : Noise(100) {
    //TODO: get random seed value
}

Noise::Noise(uint64_t seed) {
    m_seed = seed;
    
    //initialize permutation table
    for (int i = 0; i < 256; ++i) {
        m_permutations.push_back(i);
    }

    // Shuffle the permutation table based on the seed
    std::shuffle(m_permutations.begin(), m_permutations.end(), std::default_random_engine(m_seed));

    for(int i = 0; i < 512; i++) {
        m_perms[i] = m_permutations[i & 255];
    }

}

float Noise::simplexNoise(float x, float y) {
    // Placeholder implementation of 2D Simplex Noise
    // In a real scenario, this would be a full implementation of the Simplex Noise algorithm.
    return glm::simplex(glm::vec2(x, y));
}

float Noise::simplexNoise(float x, float y, float z) {
    // Placeholder implementation of 3D Simplex Noise
    // In a real scenario, this would be a full implementation of the Simplex Noise algorithm.
    return glm::simplex(glm::vec3(x, y, z));
}

double Noise::dot(const int16_t g[], double x, double y) {
    return g[0] * x + g[1] * y;
}

float Noise::generateNoise2d(float x, float y) {

    double n0;
    double n1;
    double n2;

    const double F2 = 0.366025403784439; // (sqrt(3) - 1) / 2
    double s = (x + y) * F2; //hairy factor
    int i = std::floor(x + s);
    int j = std::floor(y + s);

    const double G2 = 0.211324865405187; // (3 - sqrt(3)) / 6
    double t = (i + j) * G2;
    double X0 = i - t; // Unskew the cell origin back to (x,y) space
    double Y0 = j - t;
    double x0 = x - X0; // The x,y distances from the cell origin
    double y0 = y - Y0;
    
    //determine which simplex we are in
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if (x0 > y0) {
        i1 = 1; j1 = 0; // lower triangle
    } else {
        i1 = 0; j1 = 1; // upper triangle
    }

    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 - 1.0 + 2.0 * G2;

    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = m_perms[ii + m_perms[jj]] % 12;
    int gi1 = m_perms[ii + i1 + m_perms[jj + j1]] % 12;
    int gi2 = m_perms[ii + 1 + m_perms[jj + 1]] % 12;

    //calculate the contribution from the three corners
    double t0 = 0.5 - x0 * x0 - y0 * y0;
    if (t0 < 0) {
        n0 = 0.0;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad3[gi0], x1, y1);
    }

    double t1 = 0.5 - x1 * x1 - y1 * y1;
    if (t1 < 0) {
        n1 = 0.0;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
    }
    double t2 = 0.5 - x2 * x2 - y2 * y2;
    if (t2 < 0) {
        n2 = 0.0;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
    }

    float result = (float)(10.0 * (n0 + n1 + n2)); // Scale the result to cover the range [-1,1]
    result = (result + 1.0f) / 2.0f; // Normalize to [0,1]
    return result;

}
