#pragma once
#include <glm/vec3.hpp>
#include "Mesh.h"
#include "HgTexture.h"
#include <string>
//#include "Behaviour.h"

namespace core{

typedef enum {
    eUI = 1,
    eWorld = 2,
    eText = 3
} hgLayer;

class Entity{

    public:
        Entity(uint32_t id);
        void setMesh(Mesh mesh){m_mesh = mesh;}
        Mesh* getMesh(){return &m_mesh;}
        bool isDirty(){return m_isDirty;}
        void setDirty(bool isDirty){m_isDirty = isDirty;}
        uint32_t getId(){return m_id;}
        
        hgLayer getLayer(){return m_layer;}
        void setLayer(hgLayer layer){m_layer = layer;}

        void setTexture(HgTexture* tex){m_texture = tex;}
        uint32_t getTextureId(){return m_texture->getId();}

        void setTexturePath(std::string path){m_texturePath = path;}
        std::string getTexturePath(){return m_texturePath;}

        void setPosition(glm::vec3 pos){m_position = pos;m_isDirty = true;}
        void setRotation(glm::vec3 rot){m_rotation = rot;}
        void setScale(glm::vec3 scale){m_scale = scale;}

        glm::vec3 getPosition(){return m_position;}
        glm::vec3 getRotation(){return m_rotation;}
        glm::vec3 getScale(){return m_scale;}

        bool intersects(Entity* other);
        bool intersects(glm::vec3 pos);
        bool intersects(float x, float y, float z);
        
        virtual void onCollision(Entity* other);
        virtual void onCollision(); //specifically the mouse
        virtual void onUpdate();

    protected:
        Mesh m_mesh;

    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        HgTexture* m_texture; //TODO: have a default texture
        std::string m_texturePath;
        hgLayer m_layer = eWorld;
        uint32_t m_id;
        bool m_isDirty;

};
}