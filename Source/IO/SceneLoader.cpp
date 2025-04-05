#include "SceneLoader.h"
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

std::vector<Entity*> SceneLoader::loadScene(const char* path){
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
        Behaviour* behaviour = new Behaviour();
        if(!o.Behaviour.empty()){
            behaviour = BehaviourManager::getInstance()->getBehaviour(o.Behaviour);
        }
        GameObject* e = new GameObject(o.Id, (Behaviour*)(behaviour));
        e->setPosition(vecToVec3(o.Position));
        e->setRotation(vecToVec3(o.Rotation));
        e->setScale(vecToVec3(o.Scale));
        e->setDirty(true);
        e->setTexturePath(o.Texture);
        entities.push_back(e);
    }
    return entities;
}