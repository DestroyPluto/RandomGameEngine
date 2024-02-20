#include <stdio.h>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderingGL/RenderingEngine.h"
#include "Core/RenderingPlugin.h"


using namespace core;
using namespace rendering;


int main(int args, char** argv){

    std::unique_ptr<RenderingEngine> engine = std::make_unique<RenderingEngine>();
    std::thread renderingThread = engine->startPlugin();

    
    renderingThread.join();
    printf("closing!");
    engine->closePlugin();
    engine.reset();
    return 0;
}