#pragma once
#include <cstdint>
#include "HgError.h"
#include <thread>
//might make sense to move this into the rendering namespace... but I want it as part of the core static library so that everyone can use it.
namespace core {

class RenderingPlugin{
    public:
        virtual std::thread startPlugin() = 0;
        virtual HgError setDirtyEntities() = 0;
        virtual HgError closePlugin() = 0;
        
    protected:
        virtual HgError initPlugin() = 0;
        
};

}