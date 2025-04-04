#pragma once
#include "HgLogger.h"

namespace io {
    class Mouse {
        public:
            static Mouse* getInstance(){
                static Mouse instance;
                return &instance;
            }

            //screenPos, relative to top left
            void setPos(double x, double y){
                m_XPos = x;
                m_YPos = y;
            }

            void getScreenPos(double& x, double& y){
                x = m_XPos;
                y = m_YPos;
            }

            void setMouseButton(int key, int action);
            bool isMouseButtonDown(int key);

            typedef enum{
                MOUSE_1 = 0,
                MOUSE_2 = 1,
                MOUSE_3 = 2,
                MOUSE_4 = 3,
                MOUSE_5 = 4,
                MOUSE_6 = 5,
                MOUSE_7 = 6,
                MOUSE_8 = 7,
                MOUSE_LAST = MOUSE_8,
                MOUSE_LEFT = MOUSE_1,
                MOUSE_RIGHT = MOUSE_2,
                MOUSE_MIDDLE = MOUSE_3
            } Button;

            //default operators
            Mouse(Mouse const&) = delete;
            void operator=(Mouse const&) = delete;
        private:
            Mouse();
            virtual ~Mouse();
            double m_XPos;
            double m_YPos;
            int* m_buttons;

            enum State{
                UNKNOWN = -1,
                RELEASE = 0,
                PRESS = 1,
                REPEAT = 2 //should probably not use this lol
            };
    };
}