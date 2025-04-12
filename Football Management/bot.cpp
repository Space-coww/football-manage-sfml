#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <map>

#include "bot.h"
#include "ball.h"
#include "game.h"

std::map<char, int> actionGuide;

bool inRange(sf::Vector2f a, sf::Vector2f b, float range)
{
    float ax = std::abs(a.x);
    float ay = std::abs(a.y);

    float bx = std::abs(b.x);
    float by = std::abs(b.y);

    float x = std::max(ax, bx);
    float y = std::max(ay, by);
    float vx = std::min(ax, bx);
    float vy = std::min(ay, by);

    float xd = x - vx;
    float yd = y - vy;

    return xd < range && yd < range;
}

int aiInitialize()
{
    try
    {
        actionGuide['P'] = 1;
        actionGuide['T'] = 2;
        actionGuide['R'] = 3;
        actionGuide['S'] = 4;
    }
    catch (const std::exception&)
    {
        return 1;
    }
    return 0;
}

namespace sf {
    // Constructor without IntRect
    Bot::Bot(const Texture& texture) : Sprite(texture) {
        // Initialize your Bot here
        velocity = Vector2f(0.0f, 0.0f);
        target = Vector2f(0.0f, 0.0f);
        team = 0;
        cooldown = 0;
        energy = 100.0f;
        maxSpeed = 175.f;
        tmPos = "None";
        action = ' ';
        shoot = false;
        wantsBall = false;
        possesion = false;
        sprinting = false;
        ballVisionBlocked = false;
        bounds = getGlobalBounds();

        setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        setScale({ 0.1f, 0.1f });
        setPosition({ 100.f, 100.f });
    }

    void Bot::updateAI(float dt, RenderWindow& window, sf::Sprite* football) {
        float pickupDistance = std::abs(Ball.velocity.x) > 0.5f || std::abs(Ball.velocity.y) > 0.5f ? 50.f : 20.f; // Adjust this distance as needed
        sf::Vector2f playerCenter = getGlobalBounds().getCenter();
        sf::Vector2f ballCenter = football->getGlobalBounds().getCenter();
        float distance = std::sqrt(std::pow(playerCenter.x - ballCenter.x, 2) + std::pow(playerCenter.y - ballCenter.y, 2));
        if ((!possesion && distance < pickupDistance && !Ball.isPickedUp) && cooldown < 1)
        {
            changeAction('R');
        }

        if (!(inRange(target, getPosition(), possesion ? 800.f : 5.f)))
        {
            sf::Vector2f relativeTargetPos = target - getPosition();
            // 2. Calculate the direction vector
            float length = std::sqrt(relativeTargetPos.x * relativeTargetPos.x + relativeTargetPos.y * relativeTargetPos.y);
            sf::Vector2f direction;
            if (length != 0) {
                direction = relativeTargetPos / length;
            }
            else {
                direction = { 1.f, 0.f };
            }

            velocity = direction * (sprinting == false ? maxSpeed * 0.7f : maxSpeed);
            move(velocity * dt);
        }
        else if (wantsBall)
        {
            changeAction('R');
        }
        else if (possesion && shoot)
        {
            changeAction('S');
            possesion = false;
            shoot = false;
            wantsBall = true;
            cooldown = 200;
        }
        else if (possesion)
        {
            shoot = true;
        }
        Data.ai1Coords = getPosition();
        Data.ai1Velocity = velocity;
        Data.aiFacingDirection = velocity.x / std::abs(velocity.x);
        cooldown -= 200 * dt;
    }

    void Bot::changeAction(char newAction) {
        action = newAction;
    }

    void Bot::sprintToggle() {
        sprinting = !sprinting;
    }
}