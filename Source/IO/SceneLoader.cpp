#include "SceneLoader.h"
#include "glaze/glaze.hpp"
#include <HgLogger.h>
#include "Entity.h"
#include "glm/vec3.hpp"

using namespace io;
using namespace core;

struct vec{
    float x;
    float y;
    float z;
};

struct GameObject{
    int Id;
    vec Position;
    vec Rotation;
    vec Scale;
    std::string Texture;
    std::string Behaviour;
};

struct Scene{
    std::vector<GameObject> Objects;
};

glm::vec3 vecToVec3(vec other){
    return glm::vec3(other.x, other.y, other.z);
}

std::vector<Entity> SceneLoader::loadScene(const char* path){
    //first read in the given json file
    Scene scene{};
    auto ec = glz::read_file_json(scene, path, std::string{});
    if(ec){
        HgLogger::logError("Failed to load file %s ", path);
    }else{
        HgLogger::logDebug("Succesfully loaded file: %s", path);
    }

    std::vector<Entity> entities = std::vector<Entity>();

    for(GameObject& o : scene.Objects){
        Entity e = Entity(o.Id);
        e.setPosition(vecToVec3(o.Position));
        e.setRotation(vecToVec3(o.Rotation));
        e.setScale(vecToVec3(o.Scale));
        e.setDirty(true);
        e.setTexturePath(o.Texture);
        entities.push_back(e);
    }

    return entities;
}