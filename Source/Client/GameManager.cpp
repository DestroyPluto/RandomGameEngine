#include "GameManager.h"
#include "SceneManager.h"
#include "HgLogger.h"
#include "Keyboard.h"

using namespace client;
using namespace core;
using namespace io;

static const std::string CONFIG_STARTING_SCENE_FILE_PATH = "startingSceneFilePath";

GameManager::GameManager(std::shared_ptr<core::Config> config, RenderingPlugin* renderingPlugin){

    m_renderingPlugin = renderingPlugin;

    std::string scenePath = config->getOption(CONFIG_STARTING_SCENE_FILE_PATH, "Project/SceneOne.json");
    HgError success = SceneManager::getInstance()->loadScene(scenePath);

            if(success != HgError::eSuccess){
                HgLogger::logError("Failed to load Scene! aborting");
            }
}

void GameManager::update(){
    switch (m_currentState) {
    case ePlaying:
        updatePlaying();
        break;
    case ePaused:
        updatePaused();
        break;
    }
}

void GameManager::updatePlaying() {
    //update scene
    Scene* currentScene = SceneManager::getInstance()->getCurrentScene();
    if (currentScene)
        currentScene->update(m_renderingPlugin);

    //make sure we can exit the game lol
    if (Keyboard::getInstance()->getKeyPressed(Keyboard::KEY_ESCAPE)) {
        changeState(ePaused);
    }
}

void GameManager::updatePaused() {

    if (Keyboard::getInstance()->getKeyPressed(Keyboard::KEY_Q)) {
        m_shouldEnd = true;
    }
    //make sure we can exit the game lol
    if (Keyboard::getInstance()->getKeyPressed(Keyboard::KEY_ESCAPE)) {
        changeState(ePlaying);
    }

}

void GameManager::changeState(GameState newState) {
    m_currentState = newState;
    if (newState == ePaused) {
        HgLogger::logMsg("Game Paused. Press Q to quit.");
    }else if (newState == ePlaying) {
        HgLogger::logMsg("Game Resumed.");
    }
}