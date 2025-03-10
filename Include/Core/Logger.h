#pragma once
#include <stdio.h>
#include <utility>

#define LOG_STAMDARD_IO

namespace core{

class HgLogger{
public:
    template <typename... Args>
    static void logMsg(const char* msg, Args&&... args){log(msg, std::forward<Args>(args)...); log("\n");}
    static void logMsg(const char* msg){log(msg);log("\n");}
    
    template <typename... Args>
    static void logDebug(const char* msg, Args&&... args){log("DEBUG:");log(msg, std::forward<Args>(args)...);log("\n");}
    static void logDebug(const char* msg){log("DEBUG: ");log(msg);log("\n");}
    
    template <typename... Args>
    static void logError(const char* msg, Args&&... args){log("***ERROR***\n");log(msg, std::forward<Args>(args)...);log("\n***********\n");};
    static void logError(const char* msg){log("***ERROR***\n");log(msg);log("\n***********\n");};
    
    template <typename... Args>
    static void logWarning(const char* msg, Args&&... args){log("Warning:");log(msg, std::forward<Args>(args)...);}
    static void logWarning(const char* msg){log("Warning: ");log(msg);log("\n");}

    private:
#ifdef LOG_STAMDARD_IO
    template <typename... Args>
    static void log(const char* msg, Args&&... args){printf(msg, std::forward<Args>(args)...);;}
    static void log(const char* msg){printf(msg);}
#else
        static void log(const char* msg, Args&&... args){printf("log output undefined!");}
        static void log(const char* msg){printf("log output undefined!");}
#endif

};
}