#pragma once

namespace math{
class Point{
public:
    Point(float x, float y, float z);
    Point(double x, double y, double z);
    
    double X(){return m_x;}
    double Y(){return m_y;}
    double Z(){return m_z;}

private:
    double m_x;
    double m_y;
    double m_z;
};
}