#include "PNGLoader.h"
#include <cstring>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <Logger.h>

using namespace io;
using namespace core;

PNGLoader::PNGLoader(RenderingPlugin* plugin){
    if(plugin)
        m_plugin = plugin;
    stbi_set_flip_vertically_on_load(true);
}

HgTexture* PNGLoader::loadFromFile(const char* filePath){
    uint32_t id = 0;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    
    if(data)
       return new HgTexture(data, width, height, nrChannels, filePath);
    HgLogger::logError("failed to load png file!");

    return nullptr;
}