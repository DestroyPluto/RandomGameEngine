#include <stdio.h>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingGL/RenderingEngine.h"
#include "Core/RenderingPlugin.h"
#include "Core/Entity.h"
#include "Core/Mesh.h"

using namespace core;
using namespace rendering;


int main(int args, char** argv){

    std::unique_ptr<RenderingEngine> engine = std::make_unique<RenderingEngine>();
    std::thread renderingThread = engine->startPlugin();

    //Create a mesh of a basic square
    Entity* ent = new Entity(0);
    Mesh mesh = Mesh();

    float pointArray[] = {  -0.5, -0.5, 1.0, //1
                             0.5, -0.5, 1.0, //2
                             0.5,  0.5, 1.0, //3

                            -0.5, -0.5, 1.0, //1
                             0.5,  0.5, 1.0, //3
                            -0.5,  0.5, 1.0 //4
                            };

    std::vector<float> points = std::vector<float>(pointArray, pointArray + 8); //TODO: [pont array type]

    mesh.setPoints(points, true);

    ent->setMesh(mesh);

    std::vector<Entity*> dirtyEnts = std::vector<Entity*>();

    dirtyEnts.push_back(ent);

    //send it to the rendering Engine
    HgError err = engine->setDirtyEntities(dirtyEnts);

    if(err != HgError::eSuccess)
        printf("Something went wrong setting the dirty Ents!\n err: %d\n", err);

    renderingThread.join();
    printf("closing!\n");
    engine->closePlugin();
    engine.reset();
    return 0;
}