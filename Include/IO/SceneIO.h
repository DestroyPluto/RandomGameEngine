#pragma once
#include <vector>
#include "Entity.h"
namespace io{
    class SceneIO{
        public:
        static std::vector<core::Entity*> loadScene(const char* path);
        static void saveScene(const char* path, std::vector<core::Entity*> entities);
        private:
        struct sVec{
            float x;
            float y;
            float z;
        };
        
        struct sGameObject{
            int Id;
            sVec Position;
            sVec Rotation;
            sVec Scale;
            std::string Texture;
            std::string Behaviour;
        };
        
        struct sScene{
            std::vector<sGameObject> Objects;
        };
        
        static glm::vec3 vecToVec3(sVec other){
            return glm::vec3(other.x, other.y, other.z);
        }
        static sVec vec3ToVec(glm::vec3 other){
            return sVec {other.x, other.y, other.z};
        }
        
    };
}