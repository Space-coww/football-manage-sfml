#include <SFML/Graphics.hpp>
#include <string>

#include "bot.h"

namespace sf {
    // Constructor without IntRect
    Bot::Bot(const Texture& texture) : Sprite(texture) {
        // Initialize your Bot here
        velocity = Vector2f(0.0f, 0.0f);
        target = Vector2f(0.0f, 0.0f);
        team = 0;
        energy = 100.0f;
        maxSpeed = 5.0f;
        tmPos = "None";
        action = ' ';
        wantsBall = false;
        possesion = false;
        sprinting = false;
        ballVisionBlocked = false;
        bounds = getGlobalBounds();

        setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        setScale({ 0.1f, 0.1f });
    }

    void Bot::updateAI(float dt, RenderWindow& window) {
        if (!(target == getPosition()))
        {
            
        }
    }

    void Bot::changeAction(char newAction) {
        action = newAction;
    }

    void Bot::sprintToggle() {
        sprinting = !sprinting;
    }
}