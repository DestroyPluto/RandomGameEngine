#include "PNGLoader.h"
#include <png.h>
#include <cstring>
#include <stdlib.h>

using namespace io;
using namespace core;

PNGLoader::PNGLoader(RenderingPlugin* plugin){
    if(plugin)
        m_plugin = plugin;
}

HgError PNGLoader::loadFromFile(const char* filePath, uint32_t& id){
    printf("loading png file: %s\n", filePath);
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    if(png_image_begin_read_from_file(&image, filePath) != 0){
        png_bytep buffer;
        image.format = PNG_FORMAT_RGBA;
        size_t imageSize = PNG_IMAGE_SIZE(image);
        buffer = (png_bytep)malloc(imageSize);

        if(buffer != NULL && png_image_finish_read(&image, NULL, buffer, 0, NULL) != 0){

            //not sure if I'll need these just yet, but just in case
            size_t width = image.width;
            size_t height = image.height;
            
            m_plugin->addTexture(buffer, width, height, id);

            //this is just for testing, ignore for now
            if(png_image_write_to_file(&image, "test.png", 0, buffer, 0, NULL) != 0){
                return HgError::eSuccess;
            }
        }else{
            if(buffer = NULL){
                png_image_free(&image);
            }else{
                free(buffer);
            }
        }
    }
    printf("png to png read error: %s \n", image.message);    

    return HgError::eFailure;
}