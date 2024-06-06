#pragma once

namespace math{
class Point{
public:
    Point(float x, float y, float z);
    Point(double x, double y, double z);
    Point(double x, double y);
    Point() = delete;
    

    double X(){return m_x;}
    double Y(){return m_y;}
    double Z(){return m_z;}

    bool is2D(){return m_is2D;}

private:
    double m_x;
    double m_y;
    double m_z;
    bool m_is2D = false;
};
}