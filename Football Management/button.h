#pragma once

#include <SFML/Graphics.hpp>

class Button : public sf::Sprite
{
public:
    explicit Button(const sf::Texture& texture);
    Button(const sf::Texture& texture, const sf::IntRect& rectangle);

    bool mouseHovering(sf::RenderWindow& window);
    bool clicked(sf::RenderWindow& window);
};