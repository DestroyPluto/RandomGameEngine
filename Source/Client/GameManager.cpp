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

    m_PauseMenu = new Menu(SceneManager::getInstance()->getCurrentScene());

    Button* PauseButton = new Button(m_nextEntityId++, glm::vec3(400.0f, 300.0f, 0.0f), glm::vec3(100.0f, 100.0f, 1.0f), [this]() {
        if (m_currentState == ePlaying) {
            changeState(ePaused);
        } else {
            changeState(ePlaying);
        }
        });

    PauseButton->setText("Resume");

    Button* quitButton = new Button(m_nextEntityId++, glm::vec3(400.0f, 200.0f, 0.0f), glm::vec3(100.0f, 100.0f, 1.0f), [this]() {
        m_shouldEnd = true;
    });

    quitButton->setText("Quit");
    
    m_PauseMenu->addButton(PauseButton);
    m_PauseMenu->addButton(quitButton);
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

        m_PauseMenu->displayMenu(true);

    } else if (newState == ePlaying) {
        HgLogger::logMsg("Game Resumed.");
        m_PauseMenu->displayMenu(false);
    }
}