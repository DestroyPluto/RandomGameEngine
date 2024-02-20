#include "Mesh.h"

using namespace core;

HgError Mesh::setPoints(std::vector<float> points, bool bIs2D){
    m_points = points;
    m_b2D = bIs2D;
    return HgError::eSuccess;
}