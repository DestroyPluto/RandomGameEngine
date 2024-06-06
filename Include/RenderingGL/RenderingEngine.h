#pragma once
#include <RenderingPlugin.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <functional>

#include "RenderCommand.h"
#include "Entity.h"

namespace rendering {

class RenderingEngine : core::RenderingPlugin{

public:
        virtual std::thread startPlugin() override;
        virtual core::HgError setDirtyEntities(std::vector<core::Entity*>& entities) override;
        virtual core::HgError closePlugin() override;
        std::function<void (int, int)> m_keyCallback;

        
protected: 
        virtual core::HgError initPlugin() override;
        void renderloop();
        void handleDirtyEnts();

private:
    GLFWwindow* m_window;
    std::mutex m_RenderingMutex;
    std::vector<core::Entity*> m_dirtyEntities;
    std::unordered_map<uint32_t, RenderCommand> m_renderCommands;

    core::HgError createRenderCommand(uint32_t id, core::Mesh* mesh);
};

}