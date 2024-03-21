#pragma once
#include "Point.h"
#include <vector>

namespace math{

class PointArray{
public:
    PointArray(Point* point, size_t size);
    PointArray();
    //copies the points into the passed in array.
    void toFloatArray(float* arr, size_t& size);

private:
    std::vector<Point> m_pointArray;
};

}