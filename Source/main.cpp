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
#include "Core/Button.h"
#include "IO/PNGLoader.h"
#include "Core/HgLogger.h"
#include "Core/Config.h"
#include "IO/SceneLoader.h"
#include "Client/BehaviourManager.h"
#include "Client/TestBehaviour.h"
#include "Core/DisplayText.h"
#include "Core/GameTime.h"
#include "Client/SceneManager.h"

using namespace core;
using namespace rendering;
using namespace math;
using namespace io;
using namespace client;

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
            HgError success = SceneManager::getInstance()->loadScene("Project/GameObjects.json", (RenderingPlugin*)m_engine.get());

            if(success != HgError::eSuccess){
                HgLogger::logError("Failed to load Scene! aborting");
                shouldEnd = true;
            }
        
            Keyboard* keyboard = Keyboard::getInstance();
            
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto lastTime = std::chrono::high_resolution_clock::now();
            
            while(!shouldEnd){
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);

                double deltaTime = (double)delta.count() * 0.000000001L;
                GameTime::setDeltaTime(deltaTime);
                lastTime = currentTime;

                shouldEnd = keyboard->isKeyDown(Keyboard::KEY_ESCAPE);
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
        std::unique_ptr<RenderingEngine> m_engine = std::make_unique<RenderingEngine>();
        std::thread m_renderingThread;

        uint32_t m_count = 0;
};

int main(int args, char** argv)
{
    Game game = Game();
    game.Run();

    return 0;
}