#pragma once
#include "Button.h"
#include "Scene.h"

namespace core {
    class Menu {
    public:
        Menu(Scene* parentScene);
        virtual ~Menu() {
            //not sure if scene also expects to have ownership of the buttons... will need to test at some point.
            for (Button* button : m_buttons) {
                delete button;
            }
        }

        void addButton(Button* button);
        void removeButton(Button* button);
        const std::vector<Button*>& getButtons() const { return m_buttons; }
        void displayMenu(bool shouldDisplay);

    private:
        std::vector<Button*> m_buttons;
        Scene* m_parentScene;
        bool m_shouldDisplay = false;
    };
}