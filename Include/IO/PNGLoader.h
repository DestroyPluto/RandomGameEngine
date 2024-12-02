#pragma once
#include <HgError.h>
#include <RenderingPlugin.h>

namespace io{
    class PNGLoader{
        public:
        PNGLoader(core::RenderingPlugin* plugin);
        
        core::HgError loadFromFile(const char* filePath, uint32_t& id);

        private:
            core::RenderingPlugin* m_plugin;
    };
}