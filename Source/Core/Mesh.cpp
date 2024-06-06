#include "Mesh.h"

using namespace core;
using namespace math;

HgError Mesh::setPoints(PointArray points, bool b2D){
    m_points = points;
    m_b2D = b2D;
 
    return HgError::eSuccess;
}

HgError Mesh::setNormals(PointArray normals){
    m_Normals = normals;
    return HgError::eSuccess;
}

HgError Mesh::setUV(PointArray UV){
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
