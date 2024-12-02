#include "Button.h"
#include "PointArray.h"

using namespace math;
using namespace core;

Button::Button(uint32_t id) : Entity(id){
    m_mesh = Mesh();
    Point one = Point(-0.5, -0.5, 1.0);
    Point two = Point(0.5, -0.5, 1.0);
    Point three = Point(0.5,  0.5, 1.0);
    Point four = Point(-0.5,  0.5, 1.0);

    PointArray points = PointArray();
    //TODO: must be a better way of doing this.
    points.push_back(one);
    points.push_back(two);
    points.push_back(three);

    points.push_back(one);
    points.push_back(three);
    points.push_back(four);

    m_mesh.setPoints(points, true);
    setScale({1,0.5,0});
}