#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "HgError.h"

namespace core{

class Mesh{

public:
    HgError setPoints(std::vector<float> points, bool bIs2D);
    std::vector<float> getPoints(){return m_points;}
    bool is2D(){return m_b2D;}

private:
    std::vector<float> m_points;
    bool m_b2D;
};
}