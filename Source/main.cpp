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

using namespace core;
using namespace rendering;
using namespace math;
using namespace io;

void key_callback(int key, int action){
    Keyboard::getInstance()->setKey(key, action);
}

int main(int args, char** argv){

    std::unique_ptr<RenderingEngine> engine = std::make_unique<RenderingEngine>();
    engine->m_keyCallback = &key_callback;
    std::thread renderingThread = engine->startPlugin();

    //Create a mesh of a basic square
    Entity* ent = new Entity(0);
    Mesh mesh = Mesh();

    Point one = Point(-0.5, -0.5, 1.0);
    Point two = Point(0.5, -0.5, 1.0);
    Point three = Point(0.5,  0.5, 1.0);
    Point four = Point(-0.5,  0.5, 1.0);

    PointArray points = PointArray();
    //TODO: must be a better way of doing this.
    points.push_back(one);
    points.push_back(two);
    points.push_back(three);

    points.push_back(one);
    points.push_back(three);
    points.push_back(four);

    mesh.setPoints(points, true);

    ent->setMesh(mesh);

    std::vector<Entity*> dirtyEnts = std::vector<Entity*>();

    dirtyEnts.push_back(ent);

    //send it to the rendering Engine
    HgError err = engine->setDirtyEntities(dirtyEnts);

    if(err != HgError::eSuccess)
        printf("Something went wrong setting the dirty Ents!\n err: %d\n", err);

    bool shouldEnd = false;

    while(!shouldEnd){
        Keyboard* keyboard = Keyboard::getInstance();

        shouldEnd = keyboard->isKeyDown(Keyboard::KEY_SPACE);

        printf("main game loop! \n");
    }

    renderingThread.join();
    printf("closing!\n");
    engine->closePlugin();
    engine.reset();

    delete ent;

    return 0;
}