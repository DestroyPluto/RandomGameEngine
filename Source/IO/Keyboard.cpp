#include "Keyboard.h"
#include <stdlib.h>
#include <stdio.h>

using namespace io;

Keyboard::Keyboard(){
    m_keys = (int*)calloc(KEY_LAST, sizeof(int));
    if(!m_keys){
        printf("Failed to initialize key cache!");
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

    printf("%d is %d \n", key, action);
}
