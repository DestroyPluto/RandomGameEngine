#include "Config.h"

using namespace core;

Config::Config(){
    m_configValues = std::unordered_map<std::string, std::string>();
}

std::string Config::getOption(std::string key, std::string defaultOption){
    //only use the default option if we don't already have something assigned
    if(!m_configValues.contains(key)){
        m_configValues.insert(std::make_pair(key, defaultOption));
    }

    return getOption(key);
}

std::string Config::getOption(std::string key){
    
    if (auto option = m_configValues.find(key); option != m_configValues.end()){
        return option->second;
    }

    //no value found, so return empty
    return "";
}

void Config::setOption(std::string key, std::string option){
    m_configValues.insert_or_assign(key, option);
}

void Config::loadFromFile(std::string path){
    //do nothing for now lol
}


void Config::saveToFile(std::string path){
    //do nothing for now lol
}