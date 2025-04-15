#include <SFML/Graphics.hpp>

#include "funcs.h"
#include "ball.h"

// Checks if 2 IntRects intersect
bool intersects(sf::IntRect a, sf::IntRect b) {
    sf::Vector2i posA = a.position;
    sf::Vector2i sizeA = a.size;
    sf::Vector2i posB = b.position;
    sf::Vector2i sizeB = b.size;

    return posA.x < posB.x + sizeB.x &&
        posA.x + sizeA.x > posB.x &&
        posA.y < posB.y + sizeB.y &&
        posA.y + sizeA.y > posB.y;
}

// Checks if absolute of number is less then the required amount (for velocity checks)
bool isBelow(float number, float standard)
{
    return std::abs(number) < standard;
}

// Flip Sprites texture based on direction
void directionFlip(sf::Sprite* target, float direction, sf::Texture leftTexture, sf::Texture rightTexture)
{
    if (direction < 0)
        target->setTexture(leftTexture);
    else
        target->setTexture(rightTexture);
}

// Keep position in bounds, and bounce ball if its the ball
sf::Vector2f fixBounds(sf::Vector2f currentPosition, sf::Vector2f velocity, float dt, int screenWidth, int screenHeight, bool isBall) {
    // Calculate the displacement (how far the player moves)
    sf::Vector2f displacement = velocity * dt;

    // Calculate the new position
    sf::Vector2f newPosition = currentPosition + displacement;

    // Check for boundaries and keep the player within the screen
    if (newPosition.x < 0) {
        newPosition.x = 0; // Left boundary
        if (isBall)
            Ball.velocity.x *= -1.f;
    }
    else if (newPosition.x > screenWidth) {
        newPosition.x = screenWidth; // Right boundary
        if (isBall)
            Ball.velocity.x *= -1.f;
    }

    if (newPosition.y < 0) {
        newPosition.y = 0; // Top boundary
        if (isBall)
            Ball.velocity.y *= -1.f;
    }
    else if (newPosition.y > screenHeight) {
        newPosition.y = screenHeight; // Bottom boundary
        if (isBall)
            Ball.velocity.y *= -1.f;
    }

    return newPosition; // Return the calculated new position
}