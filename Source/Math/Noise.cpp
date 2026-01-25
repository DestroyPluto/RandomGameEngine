#include "Noise.h"
#include <cmath>
#include <glm/gtc/noise.hpp>

using namespace math;

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