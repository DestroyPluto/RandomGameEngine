#pragma once
#include <HgError.h>
#include <RenderingPlugin.h>
#include <Texture.h>

namespace io{
    class PNGLoader{
        public:
        PNGLoader(core::RenderingPlugin* plugin);
        
        core::HgTexture* loadFromFile(const char* filePath);

        private:
            core::RenderingPlugin* m_plugin;
    };
}