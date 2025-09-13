#include <stdio.h>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "RenderingGL/RenderingEngine.h"
#include "Core/RenderingPlugin.h"
#include "Core/Entity.h"
#include "Core/Mesh.h"
#include "Math/PointArray.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "IO/PNGLoader.h"
#include "Core/HgLogger.h"
#include "Core/Config.h"
#include "IO/SceneIO.h"
#include "Client/BehaviourManager.h"
#include "Core/DisplayText.h"
#include "Core/GameTime.h"
#include "Client/SceneManager.h"
#include "Client/GameManager.h"
#include <thread>

using namespace core;
using namespace rendering;
using namespace math;
using namespace io;
using namespace client;
using namespace std::chrono_literals;


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
            bool shouldEnd = false;
        
            BehaviourManager::getInstance()->RegisterClientBehaviours();
  
            Keyboard* keyboard = Keyboard::getInstance();
            
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto lastTime = std::chrono::high_resolution_clock::now();
            
            SceneManager::getInstance()->setEngine((RenderingPlugin*) m_engine.get());
            m_config->loadFromFile("config.cfg");
            GameManager game = GameManager(m_config);

            //ensure engine is initialized before we start the game
            while(!m_engine->isInitialized()){
                std::this_thread::sleep_for(10ms);
            }

            m_engine->setCameraPos(glm::vec3(0.0f, 0.0f, 3.0f));
            while(!shouldEnd){
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);

                double deltaTime = (double)delta.count() * 0.000000001L;
                GameTime::setDeltaTime(deltaTime);
                lastTime = currentTime;

                shouldEnd = keyboard->isKeyDown(Keyboard::KEY_ESCAPE) || game.shouldEnd();

                game.update();

                Scene* currentScene = SceneManager::getInstance()->getCurrentScene();
                if(currentScene)
                    currentScene->update((RenderingPlugin*)m_engine.get());

            }

            m_renderingThread.join();
            HgLogger::logMsg("closing!");
            m_engine->closePlugin();
            m_engine.reset();
        }
    private:
        std::shared_ptr<Config> m_config = std::make_shared<Config>();
        std::unique_ptr<RenderingEngine> m_engine = std::make_unique<RenderingEngine>(m_config);
        std::thread m_renderingThread;

        uint32_t m_count = 0;

};

int main(int args, char** argv)
{
    Game game = Game();
    game.Run();

    return 0;
}