#include "Noise.h"
#include <cmath>

using namespace math;

float Noise::simplexNoise(float x, float y) {
    // Placeholder implementation of 2D Simplex Noise
    // In a real scenario, this would be a full implementation of the Simplex Noise algorithm.
    return static_cast<float>(std::sin(x * 0.1f) * std::cos(y * 0.1f));
}

float Noise::simplexNoise(float x, float y, float z) {
    // Placeholder implementation of 3D Simplex Noise
    // In a real scenario, this would be a full implementation of the Simplex Noise algorithm.
    return static_cast<float>(std::sin(x * 0.1f) * std::cos(y * 0.1f) * std::sin(z * 0.1f));
}