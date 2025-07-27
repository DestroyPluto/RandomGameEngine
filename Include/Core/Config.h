#pragma once
#include <string>
#include "HgTexture.h"
#include <unordered_map>
#include <mutex>

namespace core{
    class Config{
    public:
        Config();
        std::string getOption(std::string key, std::string defaultOption);
        std::string getOption(std::string key);
        void setOption(std::string key, std::string option);
        void saveToFile(std::string path);
        void loadFromFile(std::string path);
    private:
        std::unordered_map<std::string, std::string> m_configValues;
        std::mutex m_configMutex;

    };
}