#pragma once
#include <string>
#include "HgTexture.h"
#include <unordered_map>

namespace core{
    class Config{
    public:
        static Config* getInstance(){
            static Config instance;
            return &instance;
        }
        std::string getOption(std::string key, std::string defaultOption);
        std::string getOption(std::string key);

        void setOption(std::string key, std::string option);

        void loadFromFile(std::string path);
        void saveToFile(std::string path);
    private:
        std::unordered_map<std::string, std::string> m_configValues;
        Config();
    };
}