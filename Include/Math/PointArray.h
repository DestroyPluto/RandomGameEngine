#pragma once
#include "Point.h"
#include <vector>

namespace math{

class PointArray{
public:
    PointArray(Point* point, const size_t size);
    PointArray();
    //copies the points into the passed in array.
    //Note: arr memory is owned by the caller.
    void toFloatArray(float* arr, size_t& size);
    std::vector<float> toFloatVector();
    void push_back(Point point){m_pointArray.push_back(point);};
    
    // Element access
    Point& at(size_t index) { return m_pointArray.at(index); }
    const Point& at(size_t index) const { return m_pointArray.at(index); }
    Point& operator[](size_t index) { return m_pointArray[index]; }
    const Point& operator[](size_t index) const { return m_pointArray[index]; }
    Point& getPoint(size_t index) { return m_pointArray.at(index); }
    const Point& getPoint(size_t index) const { return m_pointArray.at(index); }
    void setPoint(size_t index, const Point& point) { m_pointArray.at(index) = point; }
    
    // Capacity
    size_t size() const { return m_pointArray.size(); }
    bool empty() const { return m_pointArray.empty(); }
    void reserve(size_t capacity) { m_pointArray.reserve(capacity); }
    void resize(size_t size, const Point& value) { m_pointArray.resize(size, value); }
    
    // Modifiers
    void clear() { m_pointArray.clear(); }
    void pop_back() { m_pointArray.pop_back(); }

private:
    std::vector<Point> m_pointArray;
};

}