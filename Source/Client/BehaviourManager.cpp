#include "BehaviourManager.h"
#include "TestBehaviour.h"
#include "Player.h"
using namespace client;

void BehaviourManager::RegisterClientBehaviours(){
    registerBehaviour<TestBehaviour>("TestBehaviour");
    registerBehaviour<Player>("Player");
}