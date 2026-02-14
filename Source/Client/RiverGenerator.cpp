#include "RiverGenerator.h"
#include "Entity.h"
#include "DebugEntity.h"
#include "Core/HgLogger.h"
#include "SceneManager.h"
#include <map>
#include <unordered_set>
#include <iostream>

#define SHOW_DEBUG_NODES
#define SHOW_DEBUG_CONNECTIONS

using namespace client;
using namespace core;

// Node destructor - clean up all connections owned by this node
Node::~Node() {
    for (Connection* connection : m_connections) {
        delete connection;
    }
    m_connections.clear();
}

RiverGenerator::RiverGenerator(TerrainGenerator* terrainGenerator) 
    : m_terrainGenerator(terrainGenerator) {
    createNodes();
    HgLogger::logMsg("[RiverGenerator] Number of nodes created: %zu", m_nodes.size());

#if defined(SHOW_DEBUG_NODES) || defined(SHOW_DEBUG_CONNECTIONS)
    // Get player position once for all debug rendering
    glm::vec2 playerPos = getPlayerPosition2D();
#endif

#ifdef SHOW_DEBUG_NODES
    // Create debug entities for all nodes, but only render those within radius
    for (Node* node : m_nodes) {
        const glm::vec2& nodePos = node->getPosition();
        
        uint16_t xInt = static_cast<uint16_t>(nodePos.x);
        uint16_t yInt = static_cast<uint16_t>(nodePos.y);

        uint32_t id = CHUNK_ID_OFFSET + (xInt << 16 | yInt);

        float worldX = nodePos.x;
        float worldZ = nodePos.y;
        
        // Get actual terrain height for debug visualization
        Chunk* nodeChunk = m_terrainGenerator->getChunkAtWorldPosition(worldX, worldZ);
        float terrainY = DEFAULT_DEBUG_HEIGHT;
        if (nodeChunk) {
            terrainY = nodeChunk->getPointHeight(worldX, worldZ);
        } else {
            HgLogger::logMsg("[RiverGenerator] Warning: No chunk found for node at (%.2f, %.2f)", worldX, worldZ);
        }
        
        glm::vec3 debugPos(worldX, terrainY, worldZ);

        DebugEntity* debugEntity = new DebugEntity(id, debug_Box, debugPos);
        debugEntity->setScale(glm::vec3(NODE_DEBUG_SCALE));
        
        // Set visibility based on distance from player
        bool shouldRender = isWithinDebugRadius(nodePos, playerPos);
        debugEntity->setRenderMesh(shouldRender);
        
        m_terrainGenerator->m_parent->addChild(debugEntity);
    }
#endif // SHOW_DEBUG_NODES

#ifdef SHOW_DEBUG_CONNECTIONS
// Create debug entities for all connections, but only render those within radius
uint32_t connCounter = 0;
for (Node* node : m_nodes) {
    const glm::vec2& nodePos = node->getPosition();
    
    // Get terrain height at this node's position
    Chunk* nodeChunk = m_terrainGenerator->getChunkAtWorldPosition(nodePos.x, nodePos.y);
    float nodeTerrainHeight = DEFAULT_DEBUG_HEIGHT;
    if (nodeChunk) {
        nodeTerrainHeight = nodeChunk->getPointHeight(nodePos.x, nodePos.y);
    }
    
    for (const Connection* conn : node->getConnections()) {
        // Only draw for one direction to avoid duplicate cubes (connections are directional)
        if (node < conn->targetNode) {
            const glm::vec2& startPos = node->getPosition();
            const glm::vec2& endPos = conn->targetNode->getPosition();
            glm::vec2 debugPos2D = startPos + (endPos - startPos) * 0.25f; // 25% from start node
            glm::vec3 debugPos3D(debugPos2D.x, nodeTerrainHeight, debugPos2D.y);
                
                uint32_t connId = CONNECTION_ID_OFFSET + connCounter++;
                
                DebugEntity* debugConn = new DebugEntity(connId, debug_Box, debugPos3D);
                debugConn->setScale(glm::vec3(CONNECTION_DEBUG_SCALE));
                
                // Set visibility based on distance from player
                bool shouldRender = isWithinDebugRadius(nodePos, playerPos);
                debugConn->setRenderMesh(shouldRender);
                
                m_terrainGenerator->m_parent->addChild(debugConn);
            }
        }
    }
#endif // SHOW_DEBUG_CONNECTIONS
}

RiverGenerator::~RiverGenerator() {
    cleanupNodes();
}

void RiverGenerator::updateRivers() {
    // Currently empty
    // Eventually this will be responsible for generating river paths and modifying the terrain accordingly.
}

void RiverGenerator::updateDebugEntityVisibility() {
#if defined(SHOW_DEBUG_NODES) || defined(SHOW_DEBUG_CONNECTIONS)
    glm::vec2 playerPos = getPlayerPosition2D();
    
    #ifdef SHOW_DEBUG_NODES
    // Update visibility for all node debug entities
    for (auto& [gridKey, debugEntity] : m_nodeDebugEntities) {
        Node* node = m_grid[gridKey];
        const glm::vec2& nodePos = node->getPosition();
        bool shouldRender = isWithinDebugRadius(nodePos, playerPos);
        debugEntity->setRenderMesh(shouldRender);
    }
    #endif
    
    #ifdef SHOW_DEBUG_CONNECTIONS
    // Update visibility for all connection debug entities
    for (auto& [connKey, debugEntity] : m_connectionDebugEntities) {
        const GridKey& startGridKey = connKey.first;
        Node* startNode = m_grid[startGridKey];
        const glm::vec2& nodePos = startNode->getPosition();
        bool shouldRender = isWithinDebugRadius(nodePos, playerPos);
        debugEntity->setRenderMesh(shouldRender);
    }
    #endif
#endif
}

void RiverGenerator::createNodes() {
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
        float baseZ = chunkPos.z - chunkSize / 2.0f;
        
        for (int offsetX = 0; offsetX < chunkSize; ++offsetX) {
            for (int offsetZ = 0; offsetZ < chunkSize; ++offsetZ) {
                // Calculate grid key first to avoid floating point precision issues
                GridKey gridKey = calculateGridKey(baseX, baseZ, offsetX, offsetZ);
                
                // Derive exact position from grid key for consistency
                float gridX = static_cast<float>(gridKey.first);
                float gridZ = static_cast<float>(gridKey.second);
                glm::vec2 nodePosition(gridX, gridZ);
                
                Node* node = new Node(nodePosition);
                m_nodes.push_back(node);
                m_grid[gridKey] = node;
            }
        }
    }

    // 2. Connect each node to its 8 neighbors (across chunk boundaries)
    for (auto& [gridKey, node] : m_grid) {
        connectNodeToNeighbors(node, gridKey);
    }
}

void RiverGenerator::addNodesForChunk(Chunk* chunk) {
    int chunkSize = Chunk::CHUNK_SIZE;
    glm::vec3 chunkPos = chunk->getPosition();
    
    // Chunk mesh is centered, so calculate the corner position
    float baseX = chunkPos.x - chunkSize / 2.0f;
    float baseZ = chunkPos.z - chunkSize / 2.0f;

    if (m_nodes.empty()) {
        m_grid.clear();
    }
    
    // 1. Create nodes for this chunk
    for (int offsetX = 0; offsetX < chunkSize; ++offsetX) {
        for (int offsetZ = 0; offsetZ < chunkSize; ++offsetZ) {
            // Calculate grid key first from chunk position to avoid floating point precision issues
            GridKey gridKey = calculateGridKey(baseX, baseZ, offsetX, offsetZ);
            
            // Only create node if it doesn't already exist
            if (m_grid.find(gridKey) == m_grid.end()) {
                // Derive the exact position from the grid key to ensure consistency
                float gridX = static_cast<float>(gridKey.first);
                float gridZ = static_cast<float>(gridKey.second);
                glm::vec2 nodePosition(gridX, gridZ);
                
                Node* node = new Node(nodePosition);
                m_nodes.push_back(node);
                m_grid[gridKey] = node;
                
                // Add debug entity for node
                #ifdef SHOW_DEBUG_NODES
                uint32_t id = DYNAMIC_NODE_ID_BASE + ((uint32_t(gridKey.first) & 0xFFFFu) << 16) | (uint32_t(gridKey.second) & 0xFFFFu);
                
                float worldX = nodePosition.x;
                float worldZ = nodePosition.y;
                Chunk* nodeChunk = m_terrainGenerator->getChunkAtWorldPosition(worldX, worldZ);
                float terrainY = DEFAULT_DEBUG_HEIGHT;
                if (nodeChunk) {
                    terrainY = nodeChunk->getPointHeight(worldX, worldZ);
                } else {
                    HgLogger::logMsg("[RiverGenerator] Warning: No chunk found for node at (%.2f, %.2f)", worldX, worldZ);
                }
                
                glm::vec3 debugPos(worldX, terrainY, worldZ);
                DebugEntity* debugEntity = new DebugEntity(id, debug_Box, debugPos);
                debugEntity->setScale(glm::vec3(NODE_DEBUG_SCALE));
                
                // Set visibility based on distance from player
                glm::vec2 playerPos = getPlayerPosition2D();
                bool shouldRender = isWithinDebugRadius(nodePosition, playerPos);
                debugEntity->setRenderMesh(shouldRender);
                
                m_terrainGenerator->m_parent->addChild(debugEntity);
                m_nodeDebugEntities[gridKey] = debugEntity;
                #endif
            }
        }
    }

    // 2. Connect each new node to its 8 neighbors (across chunk boundaries)
    for (int offsetX = 0; offsetX < chunkSize; ++offsetX) {
        for (int offsetZ = 0; offsetZ < chunkSize; ++offsetZ) {
            // Use same calculation as above to ensure consistency
            GridKey gridKey = calculateGridKey(baseX, baseZ, offsetX, offsetZ);
            Node* node = m_grid[gridKey];
            
            connectNodeToNeighbors(node, gridKey);
        }
    }
}

void RiverGenerator::removeNodesForChunk(Chunk* chunk) {
    int chunkSize = Chunk::CHUNK_SIZE;
    glm::vec3 chunkPos = chunk->getPosition();
    
    // Chunk mesh is centered, so calculate the corner position
    float baseX = chunkPos.x - chunkSize / 2.0f;
    float baseZ = chunkPos.z - chunkSize / 2.0f;

    // Collect nodes to remove
    std::vector<Node*> nodesToRemove;
    std::unordered_set<Node*> nodesToRemoveSet; // For O(1) lookup
    
    for (int offsetX = 0; offsetX < chunkSize; ++offsetX) {
        for (int offsetZ = 0; offsetZ < chunkSize; ++offsetZ) {
            // Use same calculation as addNodesForChunk to ensure consistency
            GridKey gridKey = calculateGridKey(baseX, baseZ, offsetX, offsetZ);
            
            auto it = m_grid.find(gridKey);
            if (it != m_grid.end()) {
                Node* node = it->second;
                nodesToRemove.push_back(node);
                nodesToRemoveSet.insert(node);
                m_grid.erase(it);
            }
        }
    }

    // Remove connections to/from these nodes in all remaining nodes
    for (Node* node : m_nodes) {
        if (nodesToRemoveSet.find(node) != nodesToRemoveSet.end()) {
            continue; // Skip nodes that are being removed
        }
        
        auto& connections = node->getConnections();
        connections.erase(
            std::remove_if(connections.begin(), connections.end(), [&](Connection* conn) {
                // Check if target node is being removed
                bool shouldRemove = nodesToRemoveSet.find(conn->targetNode) != nodesToRemoveSet.end();
                if (shouldRemove) {
                    delete conn; // Clean up the connection
                }
                return shouldRemove;
            }),
            connections.end()
        );
    }

    // Remove from m_nodes and delete
    m_nodes.erase(
        std::remove_if(m_nodes.begin(), m_nodes.end(), [&](Node* node) {
            return nodesToRemoveSet.find(node) != nodesToRemoveSet.end();
        }),
        m_nodes.end()
    );
    
    // Delete nodes and their connections
    for (Node* node : nodesToRemove) {
        delete node;
    }
    
    // Remove debug entities for nodes
    for (Node* node : nodesToRemove) {
        const glm::vec2& nodePos = node->getPosition();
        int gridX = static_cast<int>(nodePos.x);
        int gridZ = static_cast<int>(nodePos.y);
        GridKey gridKey = {gridX, gridZ};
        
        auto it = m_nodeDebugEntities.find(gridKey);
        if (it != m_nodeDebugEntities.end()) {
            it->second->markForDestruction();
            m_nodeDebugEntities.erase(it);
        }
    }
    
    // Remove debug entities for connections
    for (Node* node : nodesToRemove) {
        const glm::vec2& nodePos = node->getPosition();
        int gridX = static_cast<int>(nodePos.x);
        int gridZ = static_cast<int>(nodePos.y);
        
        for (int dir = 0; dir < NEIGHBOR_COUNT; ++dir) {
            int neighborX = gridX + NEIGHBOR_OFFSETS_X[dir];
            int neighborZ = gridZ + NEIGHBOR_OFFSETS_Y[dir];
            
            ConnectionKey connKey1 = {{gridX, gridZ}, {neighborX, neighborZ}};
            auto connIt = m_connectionDebugEntities.find(connKey1);
            if (connIt != m_connectionDebugEntities.end()) {
                connIt->second->markForDestruction();
                m_connectionDebugEntities.erase(connIt);
            }
            
            // Also check the reverse direction to ensure both ends are cleaned up
            ConnectionKey connKey2 = {{neighborX, neighborZ}, {gridX, gridZ}};
            connIt = m_connectionDebugEntities.find(connKey2);
            if (connIt != m_connectionDebugEntities.end()) {
                connIt->second->markForDestruction();
                m_connectionDebugEntities.erase(connIt);
            }
        }
    }
}

// ===== Helper Methods =====

void RiverGenerator::cleanupNodes() {
    // Delete all nodes (which will delete their connections via Node destructor)
    for (Node* node : m_nodes) {
        delete node;
    }
    m_nodes.clear();
    m_grid.clear();
    
    // Clean up debug entities
    for (auto& [key, entity] : m_nodeDebugEntities) {
        entity->markForDestruction();
    }
    m_nodeDebugEntities.clear();
    
    for (auto& [key, entity] : m_connectionDebugEntities) {
        entity->markForDestruction();
    }
    m_connectionDebugEntities.clear();
}

void RiverGenerator::cleanupConnectionsForNode(Node* node) {
    for (Connection* connection : node->getConnections()) {
        delete connection;
    }
    node->getConnections().clear();
}

GridKey RiverGenerator::calculateGridKey(float baseX, float baseZ, int offsetX, int offsetZ) const {
    int gridKeyX = static_cast<int>(std::round(baseX)) + offsetX;
    int gridKeyZ = static_cast<int>(std::round(baseZ)) + offsetZ;
    return {gridKeyX, gridKeyZ};
}

float RiverGenerator::getTerrainHeight(const glm::vec2& worldPosition) const {
    Chunk* chunk = m_terrainGenerator->getChunkAtWorldPosition(worldPosition.x, worldPosition.y);
    
    if (chunk) {
        return chunk->getPointHeight(worldPosition.x, worldPosition.y);
    }
    
    HgLogger::logMsg("[RiverGenerator] Warning: No chunk found at world position (%.2f, %.2f)", 
                     worldPosition.x, worldPosition.y);
    return DEFAULT_TERRAIN_HEIGHT;
}

void RiverGenerator::connectNodeToNeighbors(Node* node, const GridKey& gridKey) {
    const glm::vec2& nodePosition = node->getPosition();
    float nodeHeight = getTerrainHeight(nodePosition);
    
    // Check all 8 neighboring directions
    for (int dir = 0; dir < NEIGHBOR_COUNT; ++dir) {
        int neighborX = gridKey.first + NEIGHBOR_OFFSETS_X[dir];
        int neighborZ = gridKey.second + NEIGHBOR_OFFSETS_Y[dir];
        GridKey neighborKey = {neighborX, neighborZ};
        
        auto it = m_grid.find(neighborKey);
        if (it != m_grid.end()) {
            Node* neighbor = it->second;
            const glm::vec2& neighborPosition = neighbor->getPosition();
            float neighborHeight = getTerrainHeight(neighborPosition);
            
            // Only connect to neighbors that are downhill (rivers flow downward)
            if (shouldConnectNodes(nodeHeight, neighborHeight)) {
                float weight = calculateConnectionWeight(nodeHeight, neighborHeight);
                Connection* connection = new Connection{neighbor, weight};
                node->addConnection(connection);
                
                // Add debug entity for connection (only one direction to avoid duplicates)
                #ifdef SHOW_DEBUG_CONNECTIONS
                if (gridKey.first < neighborX || (gridKey.first == neighborX && gridKey.second < neighborZ)) {
                    uint32_t connId = CONNECTION_ID_OFFSET + 
                                     (static_cast<uint16_t>(gridKey.first) << 16 | 
                                      static_cast<uint16_t>(gridKey.second));
                    
                    glm::vec2 debugPos2D = nodePosition + (neighborPosition - nodePosition) * 0.25f; // 25% from start node
                    glm::vec3 debugPos3D(debugPos2D.x, nodeHeight, debugPos2D.y);
                    
                    DebugEntity* debugConn = new DebugEntity(connId, debug_Box, debugPos3D);
                    debugConn->setScale(glm::vec3(CONNECTION_DEBUG_SCALE));
                    
                    // Set visibility based on distance from player
                    glm::vec2 playerPos = getPlayerPosition2D();
                    bool shouldRender = isWithinDebugRadius(nodePosition, playerPos);
                    debugConn->setRenderMesh(shouldRender);
                    
                    m_terrainGenerator->m_parent->addChild(debugConn);
                    
                    ConnectionKey connKey = {gridKey, neighborKey};
                    m_connectionDebugEntities[connKey] = debugConn;
                }
                #endif
            }
        }
    }
}

bool RiverGenerator::shouldConnectNodes(float nodeHeight, float neighborHeight) const {
    // Rivers flow downhill, so only connect when neighbor is lower
    return neighborHeight < nodeHeight;
}

float RiverGenerator::calculateConnectionWeight(float nodeHeight, float neighborHeight) const {
    // Weight represents the height drop (steeper = higher weight)
    return nodeHeight - neighborHeight;
}

glm::vec2 RiverGenerator::getPlayerPosition2D() const {
    Entity* playerEntity = SceneManager::getInstance()->getPlayerEntity();
    
    if (playerEntity) {
        const glm::vec3& playerPos3D = playerEntity->getPosition();
        return glm::vec2(playerPos3D.x, playerPos3D.z);
    }
    
    // Default to origin if player doesn't exist
    return glm::vec2(0.0f, 0.0f);
}

bool RiverGenerator::isWithinDebugRadius(const glm::vec2& position, const glm::vec2& playerPos) const {
    float dx = position.x - playerPos.x;
    float dz = position.y - playerPos.y;
    float distanceSquared = dx * dx + dz * dz;
    float radiusSquared = DEBUG_RENDER_RADIUS * DEBUG_RENDER_RADIUS;
    
    return distanceSquared <= radiusSquared;
}