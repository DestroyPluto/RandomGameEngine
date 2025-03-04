#pragma once
#include <glm/vec3.hpp>
#include "Mesh.h"
#include "Texture.h"

namespace core{

class Entity{

    public:
        Entity(uint32_t id);
        void setMesh(Mesh mesh){m_mesh = mesh;}
        Mesh* getMesh(){return &m_mesh;}
        bool isDirty(){return m_isDirty;}
        void setDirty(bool isDirty){m_isDirty = isDirty;}
        uint32_t getId(){return m_id;}
        
        void setTexture(HgTexture* tex){m_texture = tex;};
        uint32_t getTextureId(){return m_texture->getId();};

        void setPosition(glm::vec3 pos){m_position = pos;}
        void setRotation(glm::vec3 rot){m_rotation = rot;}
        void setScale(glm::vec3 scale){m_scale = scale;}

        glm::vec3 getPosition(){return m_position;}
        glm::vec3 getRotation(){return m_rotation;}
        glm::vec3 getScale(){return m_scale;}
    
    protected:
        Mesh m_mesh;
    
    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        HgTexture* m_texture;

        uint32_t m_id;
        bool m_isDirty;
};
}