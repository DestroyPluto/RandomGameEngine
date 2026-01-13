#include "TerrainGenerator.h"
#include "HgLogger.h"
#include <SceneManager.h>

using namespace client;
using namespace core;
using namespace math;

void TerrainGenerator::initialize(){
    m_width = 10.0f;
    m_length = 10.0f;
    m_maxHeight = 2.0f;
    HgLogger::logDebug("TerrainGenerator initialized with width: %f, length: %f, maxHeight: %f", m_width, m_length, m_maxHeight);
    createMesh();
}


void TerrainGenerator::createMesh(){
    //Mesh creation logic would go here
    HgLogger::logDebug("Creating terrain mesh...");

    Mesh terrainMesh = Mesh();
    PointArray points;
    //generate points
    m_vertexCountX = static_cast<int>(m_width) > 2 ? static_cast<int>(m_width) : 2;
    m_vertexCountZ = static_cast<int>(m_length) > 2 ? static_cast<int>(m_length) : 2;

    float widthSpacing = m_width / (float)(m_vertexCountX - 1);
    float lengthSpacing = m_length / (float)(m_vertexCountZ - 1);
    for (float x = 0.0; x < (float)m_vertexCountX; x+= widthSpacing) {
        for(float z = 0.0; z < (float)m_vertexCountZ; z+= lengthSpacing) {
            //TODO: add height generation logic here
            float y = 0.0f; // Flat terrain for now
            
            //TODO: adjust spacing based on desired width/length
            points.push_back(Point(x, y, z));
        }
    }

    HgLogger::logDebug("Generated %zu points for terrain mesh.", points.toFloatVector().size() / 3);

    //generate indices
    
    std::vector<unsigned int> indices;
    indices.reserve(static_cast<size_t>((m_vertexCountX - 1) * (m_vertexCountZ - 1) * 6));
    for(int x = 0; x < m_vertexCountX - 1; ++x) {
        for(int z = 0; z < m_vertexCountZ - 1; ++z) {
            
            int topLeft = (x * m_vertexCountZ) + z;
            int topRight = topLeft + 1;
            int bottomLeft = ((x + 1) * m_vertexCountZ) + z;
            int bottomRight = bottomLeft + 1;
     
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    HgLogger::logDebug("Generated %zu indices for terrain mesh.", indices.size());

    terrainMesh.setPoints(points, false);
    terrainMesh.setIndices(indices);

    m_parent->setDirty(true);
    m_parent->setMesh(terrainMesh);

    
}

void TerrainGenerator::update(){
    //Terrain generation logic would go here
}
