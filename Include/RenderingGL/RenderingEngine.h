#pragma once
#include <RenderingPlugin.h>

namespace rendering {

class RenderingEngine : core::RenderingPlugin{

public:
        virtual core::HgError initPlugin() override;
        virtual core::HgError setDirtyEntities() override;
        virtual core::HgError closePlugin() override; 
};

}