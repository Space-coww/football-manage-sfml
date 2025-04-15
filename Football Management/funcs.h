#pragma once

bool intersects(sf::IntRect a, sf::IntRect b);
bool isBelow(float number, float standard);

sf::Vector2f fixBounds(sf::Vector2f currentPosition, sf::Vector2f velocity, float dt, int screenWidth, int screenHeight, bool isBall = false);

void directionFlip(sf::Sprite* target, float direction, sf::Texture leftTexture, sf::Texture rightTexture);