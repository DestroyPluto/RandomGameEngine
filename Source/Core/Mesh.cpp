#include "Mesh.h"

using namespace core;
using namespace math;

HgError Mesh::setPoints(PointArray points, bool b2D){
    m_points = points;
    m_b2D = b2D;
 
    return HgError::eSuccess;
}