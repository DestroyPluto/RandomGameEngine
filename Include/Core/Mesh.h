#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "HgError.h"
#include "PointArray.h"
namespace core{

class Mesh{

public:
    HgError setPoints(math::PointArray points, bool b2D);
    math::PointArray getPoints(){return m_points;}
    bool is2D(){return m_b2D;}

private:
    math::PointArray m_points;
    bool m_b2D;
};
}