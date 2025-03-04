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
#include "IO/PNGLoader.h"
#include "core/Logger.h"

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

    //initialize png loader, and load file.
    PNGLoader loader((RenderingPlugin*)engine.get());
   
    //Create a mesh of a basic square
    //Entity* ent = new Entity(0);
    //Button* button = new Button(0);
    std::unique_ptr<Button> button = std::make_unique<Button>(0);
    button->setPosition({1,1,0});

    HgTexture* tex = loader.loadFromFile("redChecker.png");
    button->setTexture(tex);
    engine->addTexture(tex);

    std::vector<Entity*> Entities = std::vector<Entity*>();
    std::vector<Entity*> dirtyEnts = std::vector<Entity*>();
    Entities.push_back(button.get());

    //send it to the rendering Engine
    HgError err = engine->setDirtyEntities(Entities);

    if(err != HgError::eSuccess)
        HgLogger::logError("Something went wrong setting the dirty Ents!\n err: %d", err);

    bool shouldEnd = false;


    Keyboard* keyboard = Keyboard::getInstance();
    
    while(!shouldEnd){
        shouldEnd = keyboard->isKeyDown(Keyboard::KEY_ESCAPE);

        if(keyboard->isKeyDown(Keyboard::KEY_W)){
            button->setDirty(true);
        }

        dirtyEnts.clear();

        for(Entity* e : Entities){
            if(e->isDirty()){
                dirtyEnts.push_back(e);
            }
        }
        if(dirtyEnts.size() > 0){
            engine->setDirtyEntities(dirtyEnts);
        }
    }

    renderingThread.join();
    HgLogger::logMsg("closing!");
    engine->closePlugin();
    engine.reset();

    //delete ent;

    return 0;
}