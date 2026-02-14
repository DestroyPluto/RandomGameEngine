#pragma once
#include "TerrainGenerator.h"
#include <glm/vec2.hpp>
#include <vector>
#include <algorithm>
#include "chunk.h"
#include <map>
#include "DebugEntity.h"
#include <utility>

namespace client {

    class Node; // Forward declaration

    // Represents a directional connection from a node to its downstream neighbor
    // The owning node is always the source; this struct only stores the destination
    // The weight represents the height drop from source to target node
    struct Connection {
        Node* targetNode;  // The downstream/destination node
        float weight;      // Height drop (positive value)
    };

    // Represents a point in the river graph with connections to neighboring nodes
    class Node {
    public:
        explicit Node(glm::vec2 position) : m_position(position) {}
        
        // Prevent copying to avoid connection pointer issues
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        
        ~Node();

        const glm::vec2& getPosition() const { return m_position; }
        
        void addConnection(Connection* connection) {
            m_connections.push_back(connection);
        }

        void removeConnection(Connection* connection) {
            m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection), m_connections.end());
        }
        
        const std::vector<Connection*>& getConnections() const {
            return m_connections;
        }
        
        std::vector<Connection*>& getConnections() {
            return m_connections;
        }

    private:
        Node() = delete; // Disable default constructor
        
        glm::vec2 m_position;
        std::vector<Connection*> m_connections; // Connections to other nodes (potential river paths)
    };

    class TerrainGenerator; // Forward declaration

    // Type aliases for better code readability
    using GridKey = std::pair<int, int>;
    using ConnectionKey = std::pair<GridKey, GridKey>;

    class RiverGenerator {
        friend class TerrainGenerator;
    public:
        explicit RiverGenerator(TerrainGenerator* terrainGenerator);
        ~RiverGenerator();
        
        void updateRivers();
        void addNodesForChunk(Chunk* chunk);
        void removeNodesForChunk(Chunk* chunk);
        void updateDebugEntityVisibility(); // Update visibility of debug entities based on player position

    private:
        // Constants for debug visualization
        static constexpr float NODE_DEBUG_SCALE = 0.25f;
        static constexpr float CONNECTION_DEBUG_SCALE = 0.10f;
        static constexpr float DEBUG_HEIGHT_OFFSET = 1.1f;
        static constexpr uint32_t CHUNK_ID_OFFSET = 200;
        static constexpr uint32_t CONNECTION_ID_OFFSET = 1000000;
        static constexpr uint32_t DYNAMIC_NODE_ID_BASE = 0x20000000u;
        
        // Debug rendering radius around player
        static constexpr float DEBUG_RENDER_RADIUS = 32.0f;
        
        // Constants for node connectivity
        static constexpr int NEIGHBOR_COUNT = 8;
        static constexpr int NEIGHBOR_OFFSETS_X[NEIGHBOR_COUNT] = {-1, -1,  0, 1, 1,  1,  0, -1};
        static constexpr int NEIGHBOR_OFFSETS_Y[NEIGHBOR_COUNT] = { 0,  1,  1, 1, 0, -1, -1, -1};
        
        static constexpr float DEFAULT_TERRAIN_HEIGHT = 0.0f;
        static constexpr float DEFAULT_DEBUG_HEIGHT = 1.0f;
        
        TerrainGenerator* m_terrainGenerator; // Reference to the terrain generator to modify terrain for rivers

        // Core graph data structures
        std::vector<Node*> m_nodes; // All river nodes
        std::map<GridKey, Node*> m_grid; // Spatial lookup for nodes
        
        // Debug visualization tracking
        std::map<GridKey, DebugEntity*> m_nodeDebugEntities;
        std::map<ConnectionKey, DebugEntity*> m_connectionDebugEntities;
        
        // Node creation and management
        void createNodes();
        void cleanupNodes();
        void cleanupConnectionsForNode(Node* node);
        
        // Helper methods to reduce code duplication
        GridKey calculateGridKey(float baseX, float baseY, int offsetX, int offsetY) const;
        float getTerrainHeight(const glm::vec2& worldPosition) const;
        void connectNodeToNeighbors(Node* node, const GridKey& gridKey);
        bool shouldConnectNodes(float nodeHeight, float neighborHeight) const;
        float calculateConnectionWeight(float nodeHeight, float neighborHeight) const;
        
        // Debug rendering helpers
        glm::vec2 getPlayerPosition2D() const;
        bool isWithinDebugRadius(const glm::vec2& position, const glm::vec2& playerPos) const;
    };

}