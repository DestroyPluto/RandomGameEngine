#include "Point.h"

using namespace math;

Point::Point(double x, double y, double z){
    m_x = x;
    m_y = y;
    m_z = z;
}


Point::Point(float x, float y, float z){
    m_x = (double)x;
    m_y = (double)y;
    m_z = (double)z;
}