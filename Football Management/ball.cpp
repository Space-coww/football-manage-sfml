#include "ball.h"
#include "game.h"

#include <random>
#include <ctime>
#include <algorithm>

ballData Ball = {
    {0, 0},   // Ball coords
    {0, 0},   // Velocity
    false,    // Is it picked up
    0,        // Player dribbling
    0.f       // Steal timer
};

static bool isBelow(float number, float standard) {
    return std::abs(number) < standard;
}


void updateBall(int touchingPlayer, int action, int playerActing, float dt, sf::RenderWindow& window, int team) {
    /*
    Actions
    0 - Nothing
    1 - Pass
    2 - Slide
    3 - Steal
    4 - Shoot

    Priority
    1 - Pass
    2 - Slide
    3 - Steal
    4 - Shoot

    Players
    0 - none
    1 - player
    other - npc, marked on jersey

    Acting players
    0 - holder
    1 - player
    other - npc, marked on jersey
    */

    float xMouseDiff = std::clamp((sf::Mouse::getPosition().x) - Data.playerCoords.x, 0.f, 1.f);
    float yMouseDiff = std::clamp((sf::Mouse::getPosition().y) - Data.playerCoords.y, 0.f, 1.f);

    switch (action) {
    case 1: // Pass
        if (Ball.isPickedUp) {
            if (Ball.playerDribbling == playerActing) {
                Ball.playerDribbling = 0;
                Ball.isPickedUp = false;

                if (playerActing == 1) {
                    // 1. Get mouse position relative to player
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f playerPos = Data.playerCoords;
                    sf::Vector2f relativeMousePos = sf::Vector2f(mousePos) - playerPos;

                    // 2. Calculate the direction vector
                    float length = std::sqrt(relativeMousePos.x * relativeMousePos.x + relativeMousePos.y * relativeMousePos.y);
                    sf::Vector2f direction;
                    if (length != 0) {
                        direction = relativeMousePos / length;
                    }
                    else {
                        direction = { 1.f, 0.f };
                    }

                    // 3. Apply the direction vector to the ball's velocity
                    float speed = (action == 1) ? 8.f : 12.5f;
                    Ball.velocity = direction * speed;
                }
            }
        }
        break;
    case 2:
        break;

    case 3: // Steal
        std::srand(time(0));
        if (!(std::rand() % 3 == 1)) {
            Ball.playerDribbling = playerActing;
            Ball.isPickedUp = true;
        }
        break;
    case 4: // Shoot
        if (Ball.isPickedUp) {
            if (Ball.playerDribbling == playerActing) {
                Ball.playerDribbling = 0;
                Ball.isPickedUp = false;

                // 1. Get mouse position relative to player
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f playerPos = Data.playerCoords;
                sf::Vector2f relativeMousePos = sf::Vector2f(mousePos) - playerPos;

                // 2. Calculate the direction vector
                float length = std::sqrt(relativeMousePos.x * relativeMousePos.x + relativeMousePos.y * relativeMousePos.y);
                sf::Vector2f direction;
                if (length != 0) {
                    direction = relativeMousePos / length;
                }
                else {
                    direction = { 1.f, 0.f };
                }

                // 3. Apply the direction vector to the ball's velocity
                float speed = (action == 1) ? 8.f : 12.5f;
                Ball.velocity = direction * speed;
            }
        }
        break;
    default:
        if (Ball.isPickedUp) {
            Ball.velocity *= 0.f;
            if (Ball.playerDribbling == 1) {
                Ball.ballCoords = Data.playerCoords + sf::Vector2f(30.f * Data.facingDirection, 0.f);
            }
        }
        else {
            Ball.velocity -= Ball.velocity * 0.95f * dt * 1.4f;
        }
        break;
    }
}