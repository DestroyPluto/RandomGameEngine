#include "Config.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include "HgLogger.h"

using namespace core;

Config::Config(){
    m_configValues = std::unordered_map<std::string, std::string>();
}

std::string Config::getOption(std::string key, std::string defaultOption){
    //only use the default option if we don't already have something assigned
    if(!m_configValues.contains(key)){
        setOption(key, defaultOption);
    }

    return getOption(key);
}

std::string Config::getOption(std::string key){
    
    std::lock_guard<std::mutex> lock(m_configMutex); // Ensure thread safety

    if (auto option = m_configValues.find(key); option != m_configValues.end()){
        return option->second;
    }

    //no value found, so return empty
    return "";
}

void Config::setOption(std::string key, std::string option){
    std::lock_guard<std::mutex> lock(m_configMutex); // Ensure thread safety
    m_configValues.insert_or_assign(key, option);
}

void Config::loadFromFile(std::string path){
        
    if (!std::filesystem::exists(path)) {
        HgLogger::logWarning("Config file %s does not exist.", path.c_str());
        return;
    }

    std::lock_guard<std::mutex> lock(m_configMutex); // Ensure thread safety
    std::ifstream file(path);
       if (!file.is_open()) {
        HgLogger::logError("%s could not be opened for reading.", path.c_str());
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Ignore empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        size_t delimPos = line.find('=');
        if (delimPos == std::string::npos) continue; // Invalid line

        std::string key = line.substr(0, delimPos);
        std::string value = line.substr(delimPos + 1);
        m_configValues[key] = value;
    }
    file.close();
    HgLogger::logDebug("Config loaded from %s", path.c_str());
}

void Config::saveToFile(std::string path){
    std::lock_guard<std::mutex> lock(m_configMutex); // Ensure thread safety
    //build the string to write to the file
    std::ostringstream sb;
    for (const auto& [key, value] : m_configValues) {
        sb << key << "=" << value << "\n";
    }

    HgLogger::logDebug("config: %s", sb.str().c_str());

    //write the string to the file
    std::ofstream file(path);
    if (file.is_open()) {
        file << sb.str();
        file.close();
    } else {
        // Handle error opening file
        HgLogger::logError("%s could not be opened for writing.", path.c_str());
    }
    HgLogger::logDebug("Config saved to %s", path.c_str());
}