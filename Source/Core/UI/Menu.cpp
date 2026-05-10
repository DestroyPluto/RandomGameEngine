#include "Menu.h"

using namespace core;

Menu::Menu(Scene* parentScene){
    m_parentScene = parentScene;
}

void Menu::addButton(Button* button){
    m_buttons.push_back(button);
    m_parentScene->AddUIEntity(button);
    button->markForDestruction(true); //start with the button not being rendered
}

void Menu::removeButton(Button * button) {
    m_buttons.erase(std::remove(m_buttons.begin(), m_buttons.end(), button), m_buttons.end());
}

void Menu::displayMenu(bool shouldDisplay){
    for (Button* button : m_buttons) {
        button->markForDestruction(!shouldDisplay);
    }
    m_shouldDisplay = shouldDisplay;
}
