#include "SceneIO.h"
#include "glaze/glaze.hpp"
#include <HgLogger.h>
#include "Entity.h"
#include "glm/vec3.hpp"
#include "GameObject.h"
#include "Behaviour.h"
#include "BehaviourManager.h"

using namespace io;
using namespace core;
using namespace client;

std::vector<Entity*> SceneIO::loadScene(const char* path){
    //first read in the given json file
    sScene scene{};
    auto ec = glz::read_file_json(scene, path, std::string{});
    if(ec){
        HgLogger::logError("Failed to load file %s ", path);
    }else{
        HgLogger::logDebug("Succesfully loaded file: %s", path);
    }

    std::vector<Entity*> entities = std::vector<Entity*>();

    for(sGameObject& o : scene.Objects){
        Behaviour* behaviour;
        if(o.Behaviour.empty())
            behaviour = new Behaviour();
        else
            behaviour = BehaviourManager::getInstance()->getBehaviour(o.Behaviour);

        GameObject* e = new GameObject(o.Id, behaviour);

        e->setPosition(vecToVec3(o.Position));
        e->setRotation(vecToVec3(o.Rotation));
        e->setScale(vecToVec3(o.Scale));
        e->setDirty(true);
        e->setTexturePath(o.Texture);
        
        entities.push_back(e);
    }
    return entities;
}

void SceneIO::saveScene(const char* path, std::vector<core::Entity*> entities){
    
    HgLogger::logMsg("Saving scene to file: %s", path);

    sScene scene{};

    for(Entity* e : entities){
        GameObject* go = dynamic_cast<GameObject*>(e);
        
        //for now only gameobjects, no UI objects or base entitities;
        if(!go)
            continue;
        
        sGameObject obj{
            go->getId(),
            vec3ToVec(go->getPosition()),
            vec3ToVec(go->getRotation()),
            vec3ToVec(go->getScale()),
            go->getTexturePath(),
            go->getBehaviour()->getName()
        };

        scene.Objects.push_back(obj);
    }

    glz::error_ctx err = glz::write_file_json(scene, path, std::string{});

}