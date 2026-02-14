#pragma once
#include "TerrainGenerator.h"
#include <glm/vec2.hpp>
#include <vector>
#include <algorithm>
#include "chunk.h"
#include <map>
#include "DebugEntity.h"

namespace client {

    class Node; // Forward declaration

    struct Connection {
        Node* startNode;
        Node* endNode;
        float weight;
    };

    class Node {
    public:
        Node(glm::vec2 position) : m_position(position) {}
        glm::vec2 m_position;

        void addConnection(Connection* connection) {
            m_connections.push_back(connection);
        }

        void removeConnection(Connection* connection) {
            m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection), m_connections.end());
        }
        std::vector<Connection*> getConnections() const {
            return m_connections;
        }
        std::vector<Connection*> m_connections; // Connections to other nodes (potential river paths)

    private:
        Node() = delete; // Disable default constructor
    };

    class TerrainGenerator; // Forward declaration

    class RiverGenerator {
        friend class TerrainGenerator;
    public:
        RiverGenerator(TerrainGenerator* terrainGenerator);
        ~RiverGenerator();
        void updateRivers();
        void addNodesForChunk(Chunk* chunk);
        void removeNodesForChunk(Chunk* chunk);

    private:
        TerrainGenerator* m_terrainGenerator; // Reference to the terrain generator to modify terrain for rivers

        void createNodes();

        // Should this be like a graph instead?
        std::vector<Node*> m_nodes; // All river nodes
        std::map<std::pair<int, int>, Node*> m_grid;
        std::map<std::pair<int, int>, DebugEntity*> m_nodeDebugEntities;
        std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, DebugEntity*> m_connectionDebugEntities;
    };

}