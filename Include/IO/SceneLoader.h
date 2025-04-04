#pragma once
#include <vector>
#include "Entity.h"
namespace io{
    class SceneLoader{
        public:
        static std::vector<core::Entity> loadScene(const char* path);
    };
}