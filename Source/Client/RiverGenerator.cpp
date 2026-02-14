#include "RiverGenerator.h"
#include "Entity.h"
#include "DebugEntity.h"
#include "Core/HgLogger.h"
#include <map> // For std::map usage
#include <iostream>

//#define SHOW_DEBUG_NODES
//#define SHOW_DEBUG_CONNECTIONS

using namespace client;
using namespace core;

RiverGenerator::RiverGenerator(TerrainGenerator* terrainGenerator) {
    m_terrainGenerator = terrainGenerator;
    createNodes();
    HgLogger::logMsg("[RiverGenerator] Number of nodes created: %zu", m_nodes.size());

#ifdef SHOW_DEBUG_NODES
    //display the nodes as debug entities at actual terrain height
    for (Node* node : m_nodes) {
        uint16_t x_int = static_cast<uint16_t>(node->m_position.x);
        uint16_t y_int = static_cast<uint16_t>(node->m_position.y);

        const uint32_t chunkIdOffset = 200; // Arbitrary offset to avoid low IDs reserved for other entities
        uint32_t id = chunkIdOffset + (x_int << 16 | y_int);

        float wx = node->m_position.x;
        float wz = node->m_position.y;
        
        // Get actual terrain height for debug visualization
        Chunk* nodeChunk = m_terrainGenerator->getChunkAtWorldPosition(wx, wz);
        float terrainY = 1.0f; // Default fallback
        if (nodeChunk) {
            terrainY = nodeChunk->getPointHeight(wx, wz);
        }
        
        glm::vec3 debugPos(wx, terrainY, wz);

        DebugEntity* debugEntity = new DebugEntity(id, debug_Box, debugPos);
        debugEntity->setScale(glm::vec3(0.25f)); // Scale down the debug box for better visibility
        debugEntity->setRenderMesh(true);
        m_terrainGenerator->m_parent->addChild(debugEntity);
    }

#endif // SHOW_DEBUG_NODES

#ifdef SHOW_DEBUG_CONNECTIONS

    // Display a debug cube for each connection between nodes
    uint32_t connIdOffset = 1000000; // Large offset to avoid overlap with node IDs
    uint32_t connCounter = 0;
    for (Node* node : m_nodes) { 
        for (const Connection* con : node->getConnections()) {
            // Only draw for one direction to avoid duplicate cubes
            if (con->startNode < con->endNode) {
                glm::vec2 mid2d = (con->startNode->m_position + con->endNode->m_position) * 0.5f;
                glm::vec3 mid(mid2d.x, 1.1f, mid2d.y); // Slightly above ground
                
                uint32_t connId = connIdOffset + connCounter++;
                
                DebugEntity* debugConn = new DebugEntity(connId, debug_Box, mid);
                
                debugConn->setScale(glm::vec3(0.10f)); // Smaller than node cubes
                debugConn->setRenderMesh(true);
                
                m_terrainGenerator->m_parent->addChild(debugConn);
            }
        }
    }

#endif // SHOW_DEBUG_CONNECTIONS

}

RiverGenerator::~RiverGenerator() {
}

void RiverGenerator::updateRivers() {
    //atm do nothing, 
    //but eventually this will be responsible for generating river paths and modifying the terrain accordingly.

}

void RiverGenerator::createNodes() {
float nodeSpacing = 1.0f;
m_nodes.clear();
m_grid.clear();

HgLogger::logMsg("[RiverGenerator] Number of loaded chunks: %zu", m_terrainGenerator->m_loadedChunks.size());
// 1. Create nodes for all loaded chunks
for (Chunk* chunk : m_terrainGenerator->m_loadedChunks) {
    glm::vec3 chunkPos = chunk->getPosition();
    HgLogger::logMsg("[RiverGenerator] Chunk at position: (%.2f, %.2f, %.2f)", chunkPos.x, chunkPos.y, chunkPos.z);
    int chunkSize = Chunk::CHUNK_SIZE;
    // Chunk mesh is centered, so calculate the corner position
    float baseX = chunkPos.x - chunkSize / 2.0f;
    float baseY = chunkPos.z - chunkSize / 2.0f;
    for (int ix = 0; ix < chunkSize; ++ix) {
        for (int iy = 0; iy < chunkSize; ++iy) {
            // Calculate grid key first to avoid floating point precision issues
            int gridKeyX = static_cast<int>(std::round(baseX)) + ix;
            int gridKeyY = static_cast<int>(std::round(baseY)) + iy;
            // Derive exact position from grid key for consistency
            float gx = static_cast<float>(gridKeyX);
            float gy = static_cast<float>(gridKeyY);
            glm::vec2 pos(gx, gy);
            Node* node = new Node(pos);
            m_nodes.push_back(node);
            m_grid[{gridKeyX, gridKeyY}] = node;
            }
        }
    }

    // 2. Connect each node to its 8 neighbors (across chunk boundaries)
    const int dx[8] = {-1, -1,  0, 1, 1,  1,  0, -1};
    const int dy[8] = { 0,  1,  1, 1, 0, -1, -1, -1};
    for (auto& entry : m_grid) {
        Node* node = entry.second;
        int gridKeyX = entry.first.first;
        int gridKeyY = entry.first.second;
        for (int dir = 0; dir < 8; ++dir) {
            int nx = gridKeyX + dx[dir];
            int ny = gridKeyY + dy[dir];
            auto it = m_grid.find({nx, ny});
            if (it != m_grid.end()) {
                Node* neighbor = it->second;
                float weight = glm::distance(node->m_position, neighbor->m_position);
                Connection* conn = new Connection{node, neighbor, weight};
                node->addConnection(conn);
            }
        }
    }
}

void RiverGenerator::addNodesForChunk(Chunk* chunk) {
float nodeSpacing = 1.0f;
int chunkSize = Chunk::CHUNK_SIZE;
glm::vec3 chunkPos = chunk->getPosition();
// Chunk mesh is centered, so calculate the corner position
float baseX = chunkPos.x - chunkSize / 2.0f;
float baseY = chunkPos.z - chunkSize / 2.0f;

    if (m_nodes.empty()) m_grid.clear();
    uint32_t id = 0x11000011;
    // 1. Create nodes for this chunk
    for (int ix = 0; ix < chunkSize; ++ix) {
        for (int iy = 0; iy < chunkSize; ++iy) {
            // Calculate grid key first from chunk position to avoid floating point precision issues
            int gridKeyX = static_cast<int>(std::round(baseX)) + ix;
            int gridKeyY = static_cast<int>(std::round(baseY)) + iy;
            // Then derive the exact position from the grid key to ensure consistency
            float gx = static_cast<float>(gridKeyX);
            float gy = static_cast<float>(gridKeyY);
            glm::vec2 pos(gx, gy);
            if (m_grid.find({gridKeyX, gridKeyY}) == m_grid.end()) {
                Node* node = new Node(pos);
                m_nodes.push_back(node);
                m_grid[{gridKeyX, gridKeyY}] = node;
                // Add debug entity for node
                #ifdef SHOW_DEBUG_NODES
                uint32_t id = 0x20000000u + ((uint32_t(gridKeyX) & 0xFFFFu) << 16) | (uint32_t(gridKeyY) & 0xFFFFu);
                float wx = node->m_position.x;
                float wz = node->m_position.y;
                Chunk* nodeChunk = m_terrainGenerator->getChunkAtWorldPosition(wx, wz);
                float terrainY = 1.0f;
                if (nodeChunk) {
                    terrainY = nodeChunk->getPointHeight(wx, wz);
                }
                glm::vec3 debugPos(wx, terrainY, wz);
                DebugEntity* debugEntity = new DebugEntity(id, debug_Box, debugPos);
                debugEntity->setScale(glm::vec3(0.25f));
                debugEntity->setRenderMesh(true);
                m_terrainGenerator->m_parent->addChild(debugEntity);
                m_nodeDebugEntities[{gridKeyX, gridKeyY}] = debugEntity;
                #endif
            }
        }
    }

    // 2. Connect each new node to its 8 neighbors (across chunk boundaries)
    const int dx[8] = {-1, -1,  0, 1, 1,  1,  0, -1};
    const int dy[8] = { 0,  1,  1, 1, 0, -1, -1, -1};
    for (int ix = 0; ix < chunkSize; ++ix) {
        for (int iy = 0; iy < chunkSize; ++iy) {
            // Use same calculation as above to ensure consistency
            int gridKeyX = static_cast<int>(std::round(baseX)) + ix;
            int gridKeyY = static_cast<int>(std::round(baseY)) + iy;
            Node* node = m_grid[{gridKeyX, gridKeyY}];
            for (int dir = 0; dir < 8; ++dir) {
                int nx = gridKeyX + dx[dir];
                int ny = gridKeyY + dy[dir];
                auto it = m_grid.find({nx, ny});
                if (it != m_grid.end()) {
                    Node* neighbor = it->second;
                    float weight = glm::distance(node->m_position, neighbor->m_position);
                    Connection* conn = new Connection{node, neighbor, weight};
                    node->addConnection(conn);
                    // Add debug entity for connection (only one direction)
                    #ifdef SHOW_DEBUG_CONNECTIONS
                    if (gridKeyX < nx || (gridKeyX == nx && gridKeyY < ny)) {
                        uint32_t connId = 1000000 + (static_cast<uint16_t>(gridKeyX) << 16 | static_cast<uint16_t>(gridKeyY));
                        glm::vec2 mid2d = (node->m_position + neighbor->m_position) * 0.5f;
                        glm::vec3 mid(mid2d.x, 1.1f, mid2d.y);
                        DebugEntity* debugConn = new DebugEntity(connId, debug_Box, mid);
                        debugConn->setScale(glm::vec3(0.10f));
                        debugConn->setRenderMesh(true);
                        m_terrainGenerator->m_parent->addChild(debugConn);
                        m_connectionDebugEntities[{{gridKeyX, gridKeyY}, {nx, ny}}] = debugConn;
                    }
                    #endif
                }
            }
        }
    }
}

void RiverGenerator::removeNodesForChunk(Chunk* chunk) {
int chunkSize = Chunk::CHUNK_SIZE;
glm::vec3 chunkPos = chunk->getPosition();
// Chunk mesh is centered, so calculate the corner position
float baseX = chunkPos.x - chunkSize / 2.0f;
float baseY = chunkPos.z - chunkSize / 2.0f;

    // Collect nodes to remove
    std::vector<Node*> nodesToRemove;
    for (int ix = 0; ix < chunkSize; ++ix) {
        for (int iy = 0; iy < chunkSize; ++iy) {
            // Use same calculation as addNodesForChunk to ensure consistency
            int gridKeyX = static_cast<int>(std::round(baseX)) + ix;
            int gridKeyY = static_cast<int>(std::round(baseY)) + iy;
            auto it = m_grid.find({gridKeyX, gridKeyY});
            if (it != m_grid.end()) {
                Node* node = it->second;
                nodesToRemove.push_back(node);
                m_grid.erase(it);
            }
        }
    }

    // Remove connections to/from these nodes in all remaining nodes
    for (Node* node : m_nodes) {
        auto& conns = node->m_connections;
        conns.erase(std::remove_if(conns.begin(), conns.end(), [&](Connection* c) {
            return std::find(nodesToRemove.begin(), nodesToRemove.end(), c->startNode) != nodesToRemove.end() ||
                std::find(nodesToRemove.begin(), nodesToRemove.end(), c->endNode) != nodesToRemove.end();
            }), conns.end());
    }

    // Remove from m_nodes and delete
    m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(), [&](Node* n) {
        return std::find(nodesToRemove.begin(), nodesToRemove.end(), n) != nodesToRemove.end();
        }), m_nodes.end());
    for (Node* n : nodesToRemove) {
        delete n;
    }
    // Remove debug entities for nodes
    for (Node* n : nodesToRemove) {
        int gx = static_cast<int>(n->m_position.x);
        int gy = static_cast<int>(n->m_position.y);
        auto it = m_nodeDebugEntities.find({gx, gy});
        if (it != m_nodeDebugEntities.end()) {
            it->second->markForDestruction(); // Properly cleanup render commands
            m_nodeDebugEntities.erase(it);
        }
    }
    // Remove debug entities for connections
    for (Node* n : nodesToRemove) {
        int gx = static_cast<int>(n->m_position.x);
        int gy = static_cast<int>(n->m_position.y);
        const int dx[8] = {-1, -1,  0, 1, 1,  1,  0, -1};
        const int dy[8] = { 0,  1,  1, 1, 0, -1, -1, -1};
        for (int dir = 0; dir < 8; ++dir) {
            int nx = gx + dx[dir];
            int ny = gy + dy[dir];
            auto connIt = m_connectionDebugEntities.find({{gx, gy}, {nx, ny}});
            if (connIt != m_connectionDebugEntities.end()) {
                connIt->second->markForDestruction(); // Properly cleanup render commands
                m_connectionDebugEntities.erase(connIt);
            }
            // Also check the reverse direction to ensure both ends are cleaned up
            connIt = m_connectionDebugEntities.find({{nx, ny}, {gx, gy}});
            if (connIt != m_connectionDebugEntities.end()) {
                connIt->second->markForDestruction(); // Properly cleanup render commands
                m_connectionDebugEntities.erase(connIt);
            }
        }
    }
}