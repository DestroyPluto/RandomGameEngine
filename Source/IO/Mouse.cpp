#include "Mouse.h"
#include <stdlib.h>

using namespace io;

Mouse::Mouse(){
    m_XPos = -1;
    m_YPos = -1;

    m_buttons = (int*)calloc(MOUSE_LAST, sizeof(int));

}
Mouse::~Mouse(){
    free(m_buttons);
}

bool Mouse::isMouseButtonDown(int button){
    return m_buttons[button] == PRESS;
}

void Mouse::setMouseButton(int key, int action){
        //we only care if it was pressed or released
        if(action == UNKNOWN || action == REPEAT)
            return;
    
    m_buttons[key] = action;

}