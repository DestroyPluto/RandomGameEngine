#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "HgError.h"
#include "PointArray.h"
namespace core {

class Mesh{

public:
    Mesh();
    Mesh(const Mesh& other);
    HgError setPoints(math::PointArray points, bool b2D);
    HgError setNormals(math::PointArray normals);
    HgError setUV(std::vector<unsigned int> uvs);

    HgError setIndices(std::vector<unsigned int> indices);
    HgError setTextureIndices(std::vector<unsigned int> textureIndices);
    HgError setNormalIndices(std::vector<unsigned int> normalIndices);

    math::PointArray getPoints(){return m_points;};
    std::vector<unsigned int> getUvs(){return m_UV;};
    std::vector<unsigned int> getIndices() { return m_indicies; };
    math::PointArray getNormals() { return m_Normals; };
    std::vector<unsigned int> getTextureIndices() { return m_textureIndicies; };
    std::vector<unsigned int> getNormalIndices() { return m_normalIndicies; };
    
    bool is2D(){return m_b2D;}

private:
    math::PointArray m_points;
    math::PointArray m_Normals;
    std::vector<unsigned int> m_UV;

    std::vector<unsigned int> m_indicies;
    std::vector<unsigned int> m_textureIndicies;
    std::vector<unsigned int> m_normalIndicies;
    bool m_b2D;
};
}