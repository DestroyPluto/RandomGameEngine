#include "DebugEntity.h"
#include "Mesh.h"
#include "Point.h"
#include "PointArray.h"

using namespace client;
using namespace math;
using namespace core;

DebugEntity::DebugEntity(uint32_t id, DebugEntityType type, glm::vec3 position, glm::vec3 color) : core::Entity(id), m_type(type) {
    
    setPosition(position);

    PointArray points;

    switch (m_type) {
        
    case debug_Box: {
        // Create a unit cube centered at the origin
        float half = 0.5f;
        points.push_back(math::Point(-half, -half, -half)); // 0
        points.push_back(math::Point( half, -half, -half)); // 1
        points.push_back(math::Point( half,  half, -half)); // 2
        points.push_back(math::Point(-half,  half, -half)); // 3
        points.push_back(math::Point(-half, -half,  half)); // 4
        points.push_back(math::Point( half, -half,  half)); // 5
        points.push_back(math::Point( half,  half,  half)); // 6
        points.push_back(math::Point(-half,  half,  half)); // 7

        // 12 triangles (2 per face), indices for triangles
        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0, // back face
            4, 5, 6, 6, 7, 4, // front face
            0, 4, 7, 7, 3, 0, // left face
            1, 5, 6, 6, 2, 1, // right face
            3, 2, 6, 6, 7, 3, // top face
            0, 1, 5, 5, 4, 0  // bottom face
        };

        // Set all vertex colours to the provided color
        math::PointArray colours;
        for (int i = 0; i < 8; ++i) {
            colours.push_back(math::Point(color.r, color.g, color.b));
        }

        Mesh* debugMesh = new Mesh();
        debugMesh->setPoints(points, false);
        debugMesh->setIndices(indices);
        debugMesh->setColours(colours);
        setMesh(debugMesh);
        return;
    }
    case debug_line:
        // Create a simple line mesh and assign it to this entity
        // (Implementation would depend on how your rendering engine handles lines)
        //TODO: implement debug line mesh creation
        //engine doesn't actually handle lines atm.
        break;
    }

}

