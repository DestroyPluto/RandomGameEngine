#include "PointArray.h"

using namespace math;

PointArray::PointArray(){
    m_pointArray = std::vector<Point>();
}

PointArray::PointArray(Point* point, size_t size){
    PointArray();
    m_pointArray.reserve(size);
    for(size_t i = 0; i < size; size++){
        m_pointArray.push_back(point[i]);
    }
}

void PointArray::toFloatArray(float* arr, size_t& size){
    size = m_pointArray.size();
    std::vector<float> floatVec= std::vector<float>();
    floatVec.reserve(size * 3);
    for(Point p : m_pointArray){
        floatVec.push_back((double) p.X());
        floatVec.push_back((double) p.Y());
        floatVec.push_back((double) p.Z());
    }
    //set memory
    arr = (float*)malloc(size * sizeof(float));
    //copy memory to given array
    memcpy(arr, floatVec.data(), size * sizeof(float));
}