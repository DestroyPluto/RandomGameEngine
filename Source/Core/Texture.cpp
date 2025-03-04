#include "Texture.h"
#include <stdio.h>
#include <stb_image.h>
using namespace core;

HgTexture::HgTexture(unsigned char* data, int width, int height, int nChannels, const char* path){
    m_data = data;
    m_width = width;
    m_height = height;
    m_nChannels = nChannels;
    m_path = path;
}

HgTexture::~HgTexture(){
    stbi_image_free(m_data);
}