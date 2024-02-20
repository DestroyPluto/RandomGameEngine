#pragma once
#include <cstdint>
#include "HgError.h"
#include <thread>
#include "Entity.h"

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
         * clean up any rendering resources.
        */
        virtual HgError closePlugin() = 0;
        
    protected:
        /**
        * called by startPlugin() should initialise everything and begin the render loop.
        */
        virtual HgError initPlugin() = 0;
        
};

}