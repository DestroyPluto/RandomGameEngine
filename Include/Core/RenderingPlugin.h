#pragma once
#include <cstdint>
//might make sense to move this into the rendering namespace... but I want it as part of the core static library so that everyone can use it.
namespace core {

class RenderingPlugin{
    public:
        virtual int32_t initPlugin() = 0;
        virtual int32_t setDirtyEntities() = 0;
        virtual int32_t closePlugin() = 0;      
};

}