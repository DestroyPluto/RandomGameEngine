#include "GameManager.h"
#include "SceneManager.h"
#include "HgLogger.h"

using namespace client;
using namespace core;

GameManager::GameManager(){
    HgError success = SceneManager::getInstance()->loadScene("Project/SceneOne.json");

            if(success != HgError::eSuccess){
                HgLogger::logError("Failed to load Scene! aborting");
            }
}

void GameManager::update(){

}