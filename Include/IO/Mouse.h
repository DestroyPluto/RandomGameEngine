#pragma once
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
            //default operators
            Mouse(Mouse const&) = delete;
            void operator=(Mouse const&) = delete;
        private:
            Mouse();
            double m_XPos;
            double m_YPos;
    };
}