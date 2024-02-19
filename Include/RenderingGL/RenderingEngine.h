#pragma once
#include <RenderingPlugin.h>

namespace rendering {

class RenderingEngine : core::RenderingPlugin{

public:
        virtual int32_t initPlugin() override;
        virtual int32_t setDirtyEntities() override;
        virtual int32_t closePlugin() override; 

};

}