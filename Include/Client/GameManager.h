#pragma once
#include "Config.h"
#include <memory>
#include "RenderingPlugin.h"

namespace client{
    class GameManager{
        public:
        GameManager(std::shared_ptr<core::Config> config, core::RenderingPlugin* renderingPlugin);
        void update();
        bool shouldEnd(){return m_shouldEnd;}

        private:
            
            typedef enum eState {
                ePlaying,
                ePaused
            } GameState;
            
            GameState m_currentState = ePlaying;
            void updatePlaying();
            void updatePaused();
            void changeState(GameState newState);
            bool m_shouldEnd = false;
            core::RenderingPlugin* m_renderingPlugin;

    };
}