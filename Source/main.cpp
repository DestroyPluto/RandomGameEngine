#include <stdio.h>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingGL/RenderingEngine.h"
#include "Core/RenderingPlugin.h"
#include "Core/Entity.h"
#include "Core/Mesh.h"
#include "Math/PointArray.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "core/Button.cpp"

using namespace core;
using namespace rendering;
using namespace math;
using namespace io;

void key_callback(int key, int action){
    Keyboard::getInstance()->setKey(key, action);
}

void mouse_callback(double xpos, double ypos){
    Mouse::getInstance()->setPos(xpos, ypos);
}

int main(int args, char** argv){

    std::unique_ptr<RenderingEngine> engine = std::make_unique<RenderingEngine>();
    engine->m_keyCallback = &key_callback;
    engine->m_mouseCallback = & mouse_callback;
    std::thread renderingThread = engine->startPlugin();

    //Create a mesh of a basic square
    //Entity* ent = new Entity(0);
    //Button* button = new Button(0);
    std::unique_ptr<Button> button = std::make_unique<Button>(0);

    button->setPosition({1,1,0});

    std::vector<Entity*> dirtyEnts = std::vector<Entity*>();

    dirtyEnts.push_back(button.get());

    //send it to the rendering Engine
    HgError err = engine->setDirtyEntities(dirtyEnts);

    if(err != HgError::eSuccess)
        printf("Something went wrong setting the dirty Ents!\n err: %d\n", err);

    bool shouldEnd = false;

    while(!shouldEnd){
        Keyboard* keyboard = Keyboard::getInstance();

        shouldEnd = keyboard->isKeyDown(Keyboard::KEY_ESCAPE);

        if(keyboard->isKeyDown(Keyboard::KEY_W)){
            double x;
            double y;
            Mouse::getInstance()->getScreenPos(x, y);
            printf("Mouse pos: %lf, %lf \n", x, y);
        }
    }

    renderingThread.join();
    printf("closing!\n");
    engine->closePlugin();
    engine.reset();

    //delete ent;
    

    return 0;
}