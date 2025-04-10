#pragma once

namespace core{
    class GameTime{
    public:
        static void setDeltaTime(double delta){m_deltaTime = delta;}
        static double getDeltaTime(){return m_deltaTime;}
    private:
        inline static double m_deltaTime;
    };
}