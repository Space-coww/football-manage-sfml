#include "button.h"

#include <SFML/Graphics.hpp>

// Constructors from sf::Sprite (pass to the base class)
Button::Button(const sf::Texture& texture) : sf::Sprite(texture) {}

Button::Button(const sf::Texture& texture, const sf::IntRect& rectangle) : sf::Sprite(texture, rectangle) {}

// Your custom functions
bool Button::clicked(sf::RenderWindow& window)
{
    auto mouse_pos = sf::Mouse::getPosition(window);
    auto translated_pos = window.mapPixelToCoords(mouse_pos);
    if (getGlobalBounds().contains(translated_pos)) // Use getGlobalBounds() from sf::Sprite
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            return true;
    }
    return false;
}

bool Button::mouseHovering(sf::RenderWindow& window)
{
    auto mouse_pos = sf::Mouse::getPosition(window);
    auto translated_pos = window.mapPixelToCoords(mouse_pos);
    if (getGlobalBounds().contains(translated_pos)) // Use getGlobalBounds() from sf::Sprite
        return true;
    else
        return false;
}