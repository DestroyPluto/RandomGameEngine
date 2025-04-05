#pragma once
#include <string>
#include <Behaviour.h>
#include <unordered_map>
#include <utility>
#include "HgLogger.h"

namespace client{
    

    class BehaviourManager{
    public:
        static BehaviourManager* getInstance(){
            static BehaviourManager instance;
            return &instance;
        }

        void RegisterClientBehaviours();

        core::Behaviour* getBehaviour(std::string key){
            if(m_typeMap.contains(key))
                return m_typeMap[key]();
            
            core::HgLogger::logWarning("Behaviour with key: \"%s\" not found", key.c_str());
            return nullptr;
        };
        
        template<typename T> 
        void registerBehaviour(std::string key){
            if(m_typeMap.contains(key)){
                core::HgLogger::logWarning("Key \"%s\" registered twice!", key.c_str());
                return;
            }

            m_typeMap.insert(std::make_pair(key, &BehaviourManager::createInstance<T>));
        }
    private:
            std::unordered_map<std::string, core::Behaviour*(*)()> m_typeMap = std::unordered_map<std::string, core::Behaviour*(*)()>();
            
            template<typename T> 
            static core::Behaviour* createInstance(){
                return new T;
            }        
    };
}