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
#include "Core/HgLogger.h"
#include "Core/Config.h"
#include "IO/SceneLoader.h"

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
            m_entities = SceneLoader::loadScene("Project/GameObjects.json");
            //TODO handle textures better
            PNGLoader loader = PNGLoader();
            
            //load the initial textures, or just the default one if it isn't set
            for(Entity& e : m_entities){
                std::string path = e.getTexturePath().empty() ? g_defaultTexturePath : e.getTexturePath();
                HgTexture* tex =  loader.loadFromFile(path.c_str());
                e.setTexture(tex);
                m_engine->addTexture(tex);
            }

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
        
                for(Entity& e : m_entities){
                    if(e.isDirty()){
                        m_dirtyEnts.push_back(&e);
                    }
                    if(e.getLayer() == core::eUI){
                        if(e.intersects(mX,mY,0))
                            e.onCollision();
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
        std::vector<Entity> m_entities = std::vector<Entity>();
        std::vector<Entity*> m_dirtyEnts = std::vector<Entity*>();
};

int main(int args, char** argv)
{
    Game game = Game();
    game.Run();

    return 0;
}