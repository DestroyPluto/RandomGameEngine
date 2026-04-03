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
    void AddEntity(Entity* ent) { m_entities.push_back(ent); }
    void updateUI(RenderingPlugin* engine);
    void AddUIEntity(Entity* ent) { m_UIEntities.push_back(ent); }
private:
    void loadTextures(RenderingPlugin* engine);

    std::vector<Entity*> m_entities = std::vector<Entity*>();
    std::vector<Entity*> m_dirtyEnts = std::vector<Entity*>();
    std::vector<uint32_t> m_destroyedEntities = std::vector<uint32_t>();
    std::vector<Entity*> m_UIEntities = std::vector<Entity*>();
    std::vector<Entity*> m_UIDirtyEnts = std::vector<Entity*>();
    std::vector<uint32_t> m_UIDestroyedEntities = std::vector<uint32_t>();
    void updateEntity(Entity* ent, double mouseX, double mouseY);
    void updateEntityUI(Entity* ent, double mouseX, double mouseY);
    };
}