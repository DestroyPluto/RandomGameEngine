#include "BehaviourManager.h"
#include "TestBehaviour.h"

using namespace client;

void BehaviourManager::RegisterClientBehaviours(){
    registerBehaviour<TestBehaviour>("TestBehaviour");
}