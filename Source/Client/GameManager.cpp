#include "GameManager.h"
#include "SceneManager.h"
#include "HgLogger.h"

using namespace client;
using namespace core;

static const std::string CONFIG_STARTING_SCENE_FILE_PATH = "startingSceneFilePath";

GameManager::GameManager(std::shared_ptr<core::Config> config){
    std::string scenePath = config->getOption(CONFIG_STARTING_SCENE_FILE_PATH, "Project/SceneOne.json");
    HgError success = SceneManager::getInstance()->loadScene(scenePath);

            if(success != HgError::eSuccess){
                HgLogger::logError("Failed to load Scene! aborting");
            }
}

void GameManager::update(){

}