#pragma once
#include "Entity.h"
#include <string>

namespace core {

class DisplayText : public Entity {
    public:
        DisplayText(std::string text, uint32_t id) : Entity(id){
            m_text = text; 
            setLayer(eText);
        }
        std::string getText(){return m_text;}
        void setText(std::string text){m_text = text; setDirty(true);}

    private:
        std::string m_text;
};
}