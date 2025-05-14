#pragma once
#include "HgError.h"
#include "Scene.h"
#include <unordered_map>
#include <string>
#include "RenderingPlugin.h"

namespace client{
    class SceneManager{
    public:
        static SceneManager* getInstance(){
            static SceneManager instance;
            return &instance;
        }

        core::HgError loadScene(std::string path);
        core::HgError saveScene(std::string path);
        void changeScene(std::string path);
        //void setScene(std::string name);

        core::Scene* getCurrentScene(){
            if(m_currentScene)
                return m_currentScene;
            return nullptr;
        }

        void setEngine(core::RenderingPlugin* engine){m_engine = engine;}

        //TODO: should these maybe be in a better place?
        void setCameraPosition(glm::vec3 position);
        glm::vec3 getCameraPosition();

    private:
        SceneManager();
        std::unordered_map<std::string, core::Scene> m_scenes;
        core::Scene* m_currentScene;
        core::RenderingPlugin* m_engine;
    };
}