#include "TextRenderCommand.h"
#include "HgLogger.h"


#include <ft2build.h>
#include "glad/glad.h"
#include FT_FREETYPE_H


using namespace rendering;
using namespace core;

TextRenderCommand::TextRenderCommand(uint32_t globalID, std::string text){
    m_text = text;
    m_globalID = globalID;
    //setup the static character map if it hasn't been already
    if(g_characters.empty()){
        FT_Library ft;
        if(FT_Init_FreeType(&ft)){
            HgLogger::logError("Could Not init FreeType Library");
            return;
        }
        FT_Face face;
        if(FT_New_Face(ft, "Fonts/arial.ttf", 0, &face)){
            HgLogger::logError("Failed to load font");
            return;
        }
        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for(unsigned char c = 0; c < 128; c++){
            //load character glyph
            if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
                HgLogger::logError("Failed to load glyph: %c", c);
                continue;
            }
            //generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 
                0, 
                GL_RED, 
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //now store the character
            sCharacter character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<uint32_t>(face->glyph->advance.x)
            };
            g_characters.insert(std::pair<char, sCharacter>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

    }

    //create the VAO/VBO used for rendering the glyphs
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void TextRenderCommand::execute(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0); //TODO: why do we need this line?
    glBindVertexArray(m_VAO);

    std::string::const_iterator c;

    float x = m_position.x;
    float y = m_position.y;

    for(c = m_text.begin(); c != m_text.end(); c++){
        sCharacter ch = g_characters[*c];

        float xpos = x + ch.Bearing.x;// * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y);// * scale;

        float w = ch.Size.x ;//* scale;
        float h = ch.Size.y ;//* scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureId);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) ;//* scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}