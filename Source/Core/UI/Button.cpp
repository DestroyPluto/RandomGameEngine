#include "Button.h"
#include "Mesh.h"
namespace core {
    
    Button::Button(uint32_t id)
        : GameObject(id, nullptr), m_text(""), m_onClickCallback(nullptr), m_displayText(new DisplayText("",id + 1000))
    {
        // The mesh is already initialized to a 2D square in Entity constructor
        // Just set the layer to UI
        setLayer(eUI);
    }

    Button::Button(uint32_t id, glm::vec3 position, glm::vec3 scale)
        : GameObject(id, nullptr, position, glm::vec3(0.0f), scale), m_text(""), m_onClickCallback(nullptr), m_displayText(new DisplayText("", id + 1000))
    {
        setLayer(eUI);
    }

    Button::Button(uint32_t id, std::function<void()> onClickCallback)
        : GameObject(id, nullptr), m_text(""), m_onClickCallback(onClickCallback), m_displayText(new DisplayText("", id + 1000))
    {
        setLayer(eUI);
    }

    Button::Button(uint32_t id, glm::vec3 position, glm::vec3 scale, std::function<void()> onClickCallback)
        : GameObject(id, nullptr, position, glm::vec3(0.0f), scale), m_text(""), m_onClickCallback(onClickCallback), m_displayText(new DisplayText("", id + 1000))
    {
        setLayer(eUI);
    }

    Button::~Button()
    {
    }

    void Button::onUpdate()
    {
        // Call base class update
        GameObject::onUpdate();
    }

    void Button::onClick()
    {
        // Call base class click handler
        GameObject::onClick();
        
        // Execute button click callback if set
        if (m_onClickCallback) {
            m_onClickCallback();
        }
    }

    void Button::onCollision()
    {
        // Call base class collision handler
        GameObject::onCollision();
    }

    void Button::setOnClickCallback(std::function<void()> callback)
    {
        m_onClickCallback = callback;
    }

    void Button::setText(const std::string& text)
    {
        m_text = text;
        m_displayText->setText(text); // Update the display text if it exists
    }

    std::string Button::getText() const
    {
        return m_text;
    }

}
