#include "Button.h"
#include "Mesh.h"

namespace core {
    
    Button::Button(uint32_t id)
        : GameObject(id, nullptr), m_text(""), m_onClickCallback(nullptr), m_displayText(new DisplayText("",id + 1000))
    {
        setLayer(eUI);
        addChild(m_displayText);
    }

    Button::Button(uint32_t id, glm::vec3 position, glm::vec3 scale)
        : GameObject(id, nullptr, position, glm::vec3(0.0f), scale), m_text(""), m_onClickCallback(nullptr), m_displayText(new DisplayText("", id + 1500))
    {
        setLayer(eUI);
        addChild(m_displayText);
    }

    Button::Button(uint32_t id, std::function<void()> onClickCallback)
        : GameObject(id, nullptr), m_text(""), m_onClickCallback(onClickCallback), m_displayText(new DisplayText("test", id + 1500))
    {
        setLayer(eUI);
        addChild(m_displayText);
    }

    Button::Button(uint32_t id, glm::vec3 position, glm::vec3 scale, std::function<void()> onClickCallback)
        : GameObject(id, nullptr, position, glm::vec3(0.0f), scale), m_text(""), m_onClickCallback(onClickCallback), m_displayText(new DisplayText("", id + 1500))
    {
        setLayer(eUI);
        addChild(m_displayText);
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
        calculateSize();
    }

    std::string Button::getText() const
    {
        return m_text;
    }

    void Button::calculateSize()
    {
        //for now, just use magic numbers, and adjust until we get something "good enough"
        uint8_t charheight = 48;
        uint8_t padding = 10;
        float widthRatio = 0.62f; //I made this up lol.
        float charWidth = charheight * widthRatio;

        float finalWidth = m_text.length() * charWidth; 

        //make sure the text is centered
        glm::vec3 textPosition = m_displayText->getPosition();
        
        //4.0 just looks better for some reason.
        textPosition.y -= charheight / 2.0f;
        textPosition.y += padding / 2.0f;
        
        textPosition.x -= finalWidth / 2.0f; //center the text horizontally
        m_displayText->setPosition(textPosition);

        this->setScale(glm::vec3(finalWidth + (padding), charheight + padding, 1));
    }

}
