#include "Keyboard.h"
#include <stdlib.h>
#include <stdio.h>
#include "Logger.h"

using namespace io;
using namespace core;

Keyboard::Keyboard(){
    m_keys = (int*)calloc(KEY_LAST, sizeof(int));
    if(!m_keys){
        HgLogger::logError("Failed to initialize key cache!");
    }
}

Keyboard::~Keyboard(){
    free(m_keys);
}

bool Keyboard::isKeyDown(int key){
    return (m_keys[key] == PRESS);
}

void Keyboard::setKey(int key, int action){
    //we only care if it was pressed or released
    if(action == UNKNOWN || action == REPEAT)
        return;
    
    m_keys[key] = action;

    HgLogger::logDebug("%d is %d", key, action);
}
