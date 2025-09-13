#include "Keyboard.h"
#include <stdlib.h>
#include "HgLogger.h"

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
    return (m_keys[key] == PRESS || m_keys[key] == REPEAT);
}

bool Keyboard::getKeyPressed(int key){
    return m_keys[key] == PRESS;
}

void Keyboard::setKey(int key, int action){
    if (key > KEY_LAST || key < 0) {
        HgLogger::logError("Key %d is out of range!", key);
        return;
    }
    if(m_keys[key] == PRESS && action != RELEASE){
        //we already registered this key as pressed, so it must be a repeat
        m_keys[key] = REPEAT;
        return;
    }

    //we only care if it was pressed or released
    if(action == UNKNOWN || action == REPEAT)
        return;
    
    m_keys[key] = action;

    HgLogger::logDebug("%d is %d", key, action);
}
