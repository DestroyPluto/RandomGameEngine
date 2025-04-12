#pragma once
#include <vector>
#include "Entity.h"
#include "RenderingPlugin.h"

namespace core{
    class Scene{
public:
    Scene(std::vector<Entity*> entities, RenderingPlugin* engine);
    Scene() = default; //needed for std::move
    void update(RenderingPlugin* engine);
    
    std::vector<Entity*> getEntities(){return m_entities;}

private:
    void loadTextures(RenderingPlugin* engine);

    std::vector<Entity*> m_entities = std::vector<Entity*>();
    std::vector<Entity*> m_dirtyEnts = std::vector<Entity*>();

    };
}