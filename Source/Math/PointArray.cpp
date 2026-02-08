#include "PointArray.h"
#include <stdlib.h>
#include <string.h>

using namespace math;

PointArray::PointArray(){
    m_pointArray = std::vector<Point>();
}

PointArray::PointArray(Point* point, const size_t size){
    PointArray();
    m_pointArray.reserve(size);
    for(size_t i = 0; i < size; i++){
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

std::vector<float> PointArray::toFloatVector(){
    std::vector<float> floatArray = std::vector<float>();
    floatArray.reserve(m_pointArray.size() * 3); //3 = number of points. TODO: make this a variable?
    for(Point p : m_pointArray){
        floatArray.push_back(p.X());
        floatArray.push_back(p.Y());
        floatArray.push_back(p.Z());
    }
    return floatArray;

}