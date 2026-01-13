#include "BehaviourManager.h"
#include "TestBehaviour.h"
#include "Player.h"
#include "TerrainGenerator.h"
using namespace client;

void BehaviourManager::RegisterClientBehaviours(){
    registerBehaviour<TestBehaviour>("TestBehaviour");
    registerBehaviour<Player>("Player");
    registerBehaviour<TerrainGenerator>("TerrainGenerator");
}