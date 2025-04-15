#include <SFML/Graphics.hpp>
#include <cmath>

#include "movement.h"
#include "game.h"
#include "funcs.h"

// Adjusted constants for FC24-like feel
const float switchSpeedMultiplier = 2.0f; // Multiplier for fast direction switching (reduced for smoother turns)
const float friction = 8.0f;              // Friction factor (increased for quicker stops)
const float acceleration = 3.0f;          // Normal acceleration factor (reduced for smoother starts)
const float sprintMultiplier = 2.0f;      // Sprint speed multiplier (increased)

void move(float delta)
{
    if (!Data.plrVisible) return;

    sf::Vector2f input(0.f, 0.f);

    float currentSpeed = Data.speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) input.x += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) currentSpeed *= sprintMultiplier;

    // Update facing direction based on horizontal input
    if (input.x > 0.f) {
        Data.facingDirection = 1.0f;
    }
    else if (input.x < 0.f) {
        Data.facingDirection = -1.0f;
    }

    if (input.x != 0.f || input.y != 0.f) {
        // Normalize input direction if necessary
        float len = std::sqrt(input.x * input.x + input.y * input.y);
        if (len > 0) { // Avoid division by zero
            input /= len;
        }

        sf::Vector2f desiredVelocity = input * currentSpeed;

        // Determine acceleration factor
        float accelFactor = acceleration;
        // Accelerate slightly faster if changing direction sharply
        if ((desiredVelocity.x * Data.velocity.x < 0.f) || (desiredVelocity.y * Data.velocity.y < 0.f)) {
            accelFactor = switchSpeedMultiplier * acceleration; // Use multiplier relative to base acceleration
        }
        
        Data.velocity += (desiredVelocity - Data.velocity) * accelFactor * delta;
    }
    else {
        // Apply friction (simulated deceleration)
        Data.velocity -= Data.velocity * friction * delta;

        // Zero out near-zero velocity
        if (isBelow(Data.velocity.x, 0.05f)) Data.velocity.x = 0;
        if (isBelow(Data.velocity.y, 0.05f)) Data.velocity.y = 0;
    }

    // Removed diagonal speed adjustment block
}