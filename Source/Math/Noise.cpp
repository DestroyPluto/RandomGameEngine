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

    double contribution0;
    double contribution1;
    double contribution2;

    const double simplexSkewFactor = 0.366025403784439; // (sqrt(3) - 1) / 2
    double skewedSum = (x + y) * simplexSkewFactor; // Skew the input space to determine which simplex cell we're in
    int cellX = std::floor(x + skewedSum);
    int cellY = std::floor(y + skewedSum);

    const double simplexUnskewFactor = 0.211324865405187; // (3 - sqrt(3)) / 6
    double unskewedSum = (cellX + cellY) * simplexUnskewFactor;
    double cellOriginX = cellX - unskewedSum; // Unskew the cell origin back to (x,y) space
    double cellOriginY = cellY - unskewedSum;
    double distFromCellOriginX = x - cellOriginX; // The x distance from the cell origin
    double distFromCellOriginY = y - cellOriginY; // The y distance from the cell origin

    // Determine which simplex triangle we are in
    int offsetX1, offsetY1; // Offsets for second (middle) corner of simplex in (cellX,cellY) coords
    if (distFromCellOriginX > distFromCellOriginY) {
        offsetX1 = 1; offsetY1 = 0; // lower triangle
    } else {
        offsetX1 = 0; offsetY1 = 1; // upper triangle
    }

    double distFromMiddleCornerX = distFromCellOriginX - offsetX1 + simplexUnskewFactor; // Offsets for middle corner in (x,y) unskewed coords
    double distFromMiddleCornerY = distFromCellOriginY - offsetY1 + simplexUnskewFactor;
    double distFromLastCornerX = distFromCellOriginX - 1.0 + 2.0 * simplexUnskewFactor; // Offsets for last corner in (x,y) unskewed coords
    double distFromLastCornerY = distFromCellOriginY - 1.0 + 2.0 * simplexUnskewFactor;

    // Work out the hashed gradient indices of the three simplex corners
    int cellXmod = cellX & 255;
    int cellYmod = cellY & 255;
    int gradIndex0 = m_perms[cellXmod + m_perms[cellYmod]] % 12;
    int gradIndex1 = m_perms[cellXmod + offsetX1 + m_perms[cellYmod + offsetY1]] % 12;
    int gradIndex2 = m_perms[cellXmod + 1 + m_perms[cellYmod + 1]] % 12;

    // Calculate the contribution from the three corners
    double t0 = 0.5 - distFromCellOriginX * distFromCellOriginX - distFromCellOriginY * distFromCellOriginY;
    if (t0 < 0) {
        contribution0 = 0.0;
    } else {
        t0 *= t0;
        contribution0 = t0 * t0 * dot(grad3[gradIndex0], distFromCellOriginX, distFromCellOriginY);
    }

    double t1 = 0.5 - distFromMiddleCornerX * distFromMiddleCornerX - distFromMiddleCornerY * distFromMiddleCornerY;
    if (t1 < 0) {
        contribution1 = 0.0;
    } else {
        t1 *= t1;
        contribution1 = t1 * t1 * dot(grad3[gradIndex1], distFromMiddleCornerX, distFromMiddleCornerY);
    }
    double t2 = 0.5 - distFromLastCornerX * distFromLastCornerX - distFromLastCornerY * distFromLastCornerY;
    if (t2 < 0) {
        contribution2 = 0.0;
    } else {
        t2 *= t2;
        contribution2 = t2 * t2 * dot(grad3[gradIndex2], distFromLastCornerX, distFromLastCornerY);
    }

    float result = (float)(70.0 * (contribution0 + contribution1 + contribution2)); // Scale the result to cover the range [-1,1]
    //result = (result + 1.0f) / 2.0f; // Normalize to [0,1]
    return result;

}
