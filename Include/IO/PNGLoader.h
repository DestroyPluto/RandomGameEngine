#pragma once
#include <HgError.h>
#include <RenderingPlugin.h>
#include "HgTexture.h"

namespace io{
    class PNGLoader{
        public:
        PNGLoader();
        
        core::HgTexture* loadFromFile(const char* filePath);
    };
}