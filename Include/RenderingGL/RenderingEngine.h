#pragma once
#include <RenderingPlugin.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Entity.h"

namespace rendering {

class RenderingEngine : core::RenderingPlugin{

public:
        virtual std::thread startPlugin() override ;
        virtual core::HgError setDirtyEntities(std::vector<core::Entity*> entities) override;
        virtual core::HgError closePlugin() override;
protected: 
        virtual core::HgError initPlugin() override;
        void renderloop();

private:
    GLFWwindow* m_window;
};

}