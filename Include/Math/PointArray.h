#pragma once
#include "Point.h"
#include <vector>

namespace math{

class PointArray{
public:
    PointArray(Point* point, size_t size);
    PointArray();
    //copies the points into the passed in array.
    //Note: arr memory is owned by the caller.
    void toFloatArray(float* arr, size_t& size);
    void add(Point point){m_pointArray.push_back(point);};

private:
    std::vector<Point> m_pointArray;
};

}