#pragma once
#include "GameObject.h"
#include <functional>
#include <string>
#include "DisplayText.h"


namespace core {
    class Button : public GameObject {
        public:

            Button(uint32_t id);
            Button(uint32_t id, glm::vec3 position, glm::vec3 scale);
            Button(uint32_t id, std::function<void()> onClickCallback);
            Button(uint32_t id, glm::vec3 position, glm::vec3 scale, std::function<void()> onClickCallback);
            virtual ~Button();
            virtual void onUpdate() override;
            virtual void onClick() override;
            virtual void onCollision() override;
            void setOnClickCallback(std::function<void()> callback);
            void setText(const std::string& text);
            std::string getText() const;
    protected:
        std::string m_text;
        std::function<void()> m_onClickCallback;
        DisplayText* m_displayText; // Optional: for rendering the button's text
    private:
        void calculateSize();
    };
}