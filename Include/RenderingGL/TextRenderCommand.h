#pragma once
#include <cstdint>
#include <string>
#include <glm/mat4x4.hpp>
#include <map>

namespace rendering{
    struct sCharacter{
        uint32_t TextureId;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        uint32_t Advance;
    };

    static std::map<char, sCharacter> g_characters;

    class TextRenderCommand{
    public:
        TextRenderCommand(uint32_t globalId, std::string text);
        void execute();
        
        //TODO: rendering key (I don't actually do anything with it atm, 
        //                     so will deal with it when I actually 
        //                     need it lol)
        
        void setPosition(glm::vec3 position){m_position = position;}
        void setText(std::string text){m_text = text;}
    private:
            uint32_t m_globalID;
            std::string m_text;
            uint32_t m_VAO;
            uint32_t m_VBO;
            glm::vec3 m_position;
    };
}