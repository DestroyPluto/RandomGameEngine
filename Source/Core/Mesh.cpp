#include "Mesh.h"

using namespace core;
using namespace math;

Mesh::Mesh(){
    //defaults to a 2D square
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

    setPoints(points, true);

    std::vector<unsigned int> uvs = std::vector<unsigned int>({ 0, 0,
                                                                1, 0,
                                                                1, 1,
                                                                0, 0,
                                                                1, 1,
                                                                0, 1, });
    
    setUV(uvs);
}

Mesh::Mesh(const Mesh& other){
    setPoints(other.m_points, other.m_b2D);
    setNormals(other.m_Normals);
    setUV(other.m_UV);
    setIndices(other.m_indicies);
    setTextureIndices(other.m_textureIndicies);
    setNormalIndices(other.m_normalIndicies);
}

HgError Mesh::setPoints(PointArray points, bool b2D){
    m_points = points;
    m_b2D = b2D;
 
    return HgError::eSuccess;
}

HgError Mesh::setNormals(PointArray normals){
    m_Normals = normals;
    return HgError::eSuccess;
}

HgError Mesh::setUV(std::vector<unsigned int> UV){
    m_UV = UV;
    return HgError::eSuccess;
}

HgError Mesh::setIndices(std::vector<unsigned int> indices){
    m_indicies = indices;
    return HgError::eSuccess;
}

HgError Mesh::setTextureIndices(std::vector<unsigned int> textureIndices){
    m_textureIndicies = textureIndices;
    return HgError::eSuccess;
}

HgError Mesh::setNormalIndices(std::vector<unsigned int> normalIndices){
    m_normalIndicies = normalIndices;
    return HgError::eSuccess;
}

HgError Mesh::setColours(PointArray colours){
    m_colours = colours;
    return HgError::eSuccess;
}