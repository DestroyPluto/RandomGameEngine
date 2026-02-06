#pragma once
#include <cstdint>
#include "HgError.h"
#include <thread>
#include "Entity.h"
#include "Config.h"
#include <memory>
//might make sense to move this into the rendering namespace... but I want it as part of the core static library so that everyone can use it.
namespace core {

class RenderingPlugin{
    public:
        /**
         * starts a thread that inits the rendering engine and starts the render loop.
        */
       
        virtual std::thread startPlugin() = 0;
        /**
         * Note: entities is not owned by the rendering plugin. the relevent info is copied over only.
        */

        virtual HgError setDirtyEntities(std::vector<Entity*>& entities) = 0;

        /**
        * removes rendercommands that match the entity ids passed in.
        */

        virtual HgError destroyEntities(std::vector<uint32_t>& entIds) = 0;

        /**
         * load a texture/image into the rendering engine
         * [out] id - the image/texture ID
         */
        virtual HgError addTexture(core::HgTexture* tex) = 0;

        /**
         * clean up any rendering resources.
        */
        virtual HgError closePlugin() = 0;
        
        /**
        * Set the Camera position, in world coordinates
        */
        virtual HgError setCameraPos(glm::vec3 translation) = 0;
        
        /**
         * get the Camera position, in world coordinates
         */
        virtual glm::vec3 getCameraPos() = 0;

        /**
         * get the config for the rendering plugin.
         */
        virtual	std::shared_ptr<Config> getConfig()  = 0;

    protected:
        /**
        * called by startPlugin() should initialise everything and begin the render loop.
        */
        virtual HgError initPlugin() = 0;
        
};

}