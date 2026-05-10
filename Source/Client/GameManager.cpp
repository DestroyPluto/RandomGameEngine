#include "GameManager.h"
#include "SceneManager.h"
#include "HgLogger.h"
#include "Keyboard.h"
#include <vector>

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

    m_pauseButton = new Button(m_nextEntityId++, glm::vec3(400.0f, 300.0f, 0.0f), glm::vec3(100.0f, 100.0f, 1.0f), [this]() {
        if (m_currentState == ePlaying) {
            changeState(ePaused);
        } else {
            changeState(ePlaying);
        }
        });
    m_pauseButton->setText("Resume");

    m_quitButton = new Button(m_nextEntityId++, glm::vec3(400.0f, 200.0f, 0.0f), glm::vec3(100.0f, 100.0f, 1.0f), [this]() {
        m_shouldEnd = true;
    });

    m_quitButton->setText("Quit");
    
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
    
    SceneManager::getInstance()->getCurrentScene()->updateUI(m_renderingPlugin);
}

void GameManager::changeState(GameState newState) {
    m_currentState = newState;
    if (newState == ePaused) {
        HgLogger::logMsg("Game Paused. Press Q to quit.");
        SceneManager::getInstance()->getCurrentScene()->AddUIEntity(m_pauseButton);
        SceneManager::getInstance()->getCurrentScene()->AddUIEntity(m_quitButton);
        m_pauseButton->markForDestruction(false);
        m_quitButton->markForDestruction(false);
        m_pauseButton->setDirty(true);
        m_quitButton->setDirty(true);

    }else if (newState == ePlaying) {
        HgLogger::logMsg("Game Resumed.");
        m_pauseButton->markForDestruction(true);
        m_quitButton->markForDestruction(true);
        m_pauseButton->setDirty(true);
        m_quitButton->setDirty(true);
    }
}