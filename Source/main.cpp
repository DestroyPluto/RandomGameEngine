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
#include "Core/Button.h"
#include "IO/PNGLoader.h"
#include "Core/Logger.h"
#include "Core/Config.h"

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

void mouse_button_callback(int button, int action){
    Mouse::getInstance()->setMouseButton(button, action);
}

class Game{

    public:    
    	Game(){ 
            m_engine->m_keyCallback = &key_callback;
            m_engine->m_mouseCallback = &mouse_callback;
            m_engine->m_mouseButtonCallback = &mouse_button_callback;
            m_renderingThread = m_engine->startPlugin();
        }
        void Run(){
            std::unique_ptr<Button> button = std::make_unique<Button>(0);
            button->setPosition({0.0f,0.0f,0});
        
            //std::unique_ptr<Entity> entity = std::make_unique<Entity>(1);
            //entity->setPosition({1.0f,0.0f,0.0f});
            //entity->setScale({0.5f,0.5f,1.0f});
            
            //TODO handle textures better
            PNGLoader loader = PNGLoader();
            HgTexture* tex = loader.loadFromFile(g_defaultTexturePath.c_str());
            //button->setTexture(tex);
            //entity->setTexture(tex);  
            m_engine->addTexture(tex);

            //TODO: move these into an Entity Manager?
          
            //Entities.push_back(button.get());
            //m_entities.push_back(entity.get());
            
            //send it to the rendering Engine
            HgError err = m_engine->setDirtyEntities(m_entities);
        
            if(err != HgError::eSuccess)
                HgLogger::logError("Something went wrong setting the dirty Ents!\n err: %d", err);
        
            bool shouldEnd = false;
        
            Keyboard* keyboard = Keyboard::getInstance();
            
            while(!shouldEnd){
                shouldEnd = keyboard->isKeyDown(Keyboard::KEY_ESCAPE);
        
                if(keyboard->isKeyDown(Keyboard::KEY_W)){
                    button->setDirty(true);
                }
        
                m_dirtyEnts.clear();
                double mX, mY;
                Mouse::getInstance()->getScreenPos(mX, mY);
        
                for(Entity* e : m_entities){
                    if(e->isDirty()){
                        m_dirtyEnts.push_back(e);
                    }
                    if(e->getLayer() == core::eUI){
                        if(e->intersects(mX,mY,0))
                            e->onCollision();
                    }
                }
                if(m_dirtyEnts.size() > 0){
                    m_engine->setDirtyEntities(m_dirtyEnts);
                }
            }
        
            m_renderingThread.join();
            HgLogger::logMsg("closing!");
            m_engine->closePlugin();
            m_engine.reset();
        }
    private:
        std::unique_ptr<RenderingEngine> m_engine = std::make_unique<RenderingEngine>();
        std::thread m_renderingThread;
        std::vector<Entity*> m_entities = std::vector<Entity*>();
        std::vector<Entity*> m_dirtyEnts = std::vector<Entity*>();
};

int main(int args, char** argv)
{
    Game game = Game();
    game.Run();

    return 0;
}