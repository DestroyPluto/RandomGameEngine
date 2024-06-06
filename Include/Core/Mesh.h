#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "HgError.h"
#include "PointArray.h"
namespace core{

class Mesh{

public:
    HgError setPoints(math::PointArray points, bool b2D);
    HgError setNormals(math::PointArray normals);
    HgError setUV(math::PointArray UVs);

    HgError setIndices(std::vector<unsigned int> indices);
    HgError setTextureIndices(std::vector<unsigned int> textureIndices);
    HgError setNormalIndices(std::vector<unsigned int> normalIndices);

    math::PointArray getPoints(){return m_points;}
    bool is2D(){return m_b2D;}

private:
    math::PointArray m_points;
    math::PointArray m_Normals;
    math::PointArray m_UV;

    std::vector<unsigned int> m_indicies;
    std::vector<unsigned int> m_textureIndicies;
    std::vector<unsigned int> m_normalIndicies;
    bool m_b2D;
};
}