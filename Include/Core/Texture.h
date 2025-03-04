#pragma once
#include <cstdint>
namespace core{
    class HgTexture{
        public:
            HgTexture(unsigned char* data, int width, int height, int nChannels, const char* path);
            
            int getWidth(){return m_width;}
            int getHeight(){return m_height;}
            unsigned char* getData(){return m_data;}
            const char* getPath(){return m_path;}
            uint32_t getId(){return m_id;}
            void setId(uint32_t id){
                m_id = id;
            }
            virtual ~HgTexture();
        private:

            unsigned char* m_data;
            int m_width;
            int m_height;
            int m_nChannels;
            uint32_t m_id = 0;
            const char* m_path;
    };
}