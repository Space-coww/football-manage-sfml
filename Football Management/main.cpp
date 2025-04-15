#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#pragma region headers

#include "game.h"
#include "button.h"
#include "movement.h"
#include "ball.h"
#include "goal.h"
#include "bot.h"
#include "funcs.h"

#pragma endregion

sf::Clock deltaClock;

int main()
{

    // Window

    sf::RenderWindow window(sf::VideoMode({ 1000, 1000 }), "Footballs!", sf::State::Fullscreen);

    window.setFramerateLimit(60);

    window.setMouseCursorVisible(true);

    sf::Vector2u screenSize = sf::VideoMode::getFullscreenModes()[0].size;
    float screenWidth = static_cast<float>(screenSize.x);
    float screenHeight = static_cast<float>(screenSize.y);

    initiateGoals(screenWidth, screenHeight);

    if (!(aiInitialize() == 0))
    {
        std::cerr << "Error Initializing AI" << std::endl;
        return -1;
    }

#pragma region Texture Loading...

    sf::Texture texturePlayerRight;
    if (!texturePlayerRight.loadFromFile("player.png")) {
        std::cerr << "Error loading image!" << std::endl;
        return -1; // Exit with an error code
    }

    sf::Texture texturePlayerLeft; // Texture for player facing left
    if (!texturePlayerLeft.loadFromFile("player2.png")) {
        std::cerr << "Error loading player2.png!" << std::endl;
        return -1; // Exit with an error code
    }

    sf::Texture texture2;
    if (!texture2.loadFromFile("play.png")) {
        std::cerr << "Error loading image!" << std::endl;
        return -1; // Exit with an error code
    }

    sf::Texture texture3;
    if (!texture3.loadFromFile("football.png")) {
        std::cerr << "Error loading image!" << std::endl;
        return -1; // Exit with an error code
    }

    sf::Texture texture4;
    if (!texture4.loadFromFile("field.png")) {
        std::cerr << "Error loading image!" << std::endl;
        return -1; // Exit with an error code
    }

    // Width and Height of screen for reusability

    int width = window.getSize().x;
    int height = window.getSize().y;

#pragma endregion

#pragma region Sprite Initialization

    // Player Initialization

    sf::Sprite player(texturePlayerRight);

    sf::FloatRect playerBounds = player.getGlobalBounds();

    player.setOrigin({ playerBounds.size.x / 2.f, playerBounds.size.y / 2.f });
    player.setScale({ 0.1f, 0.1f });

    // ai initialization

    sf::Bot ai1(texturePlayerLeft);

    ai1.wantsBall = true;

    ai1.target = Ball.ballCoords;

    // Play Button initialization.

    Button playButton(texture2); // Custom class using sf::sprite that detects mouse hovering and clicking

    sf::FloatRect playButtonBounds = playButton.getGlobalBounds();

    playButton.setOrigin({ playButtonBounds.size.x / 2.f, playButtonBounds.size.y / 2.f }); // Center origin
    playButton.setPosition({ width * 0.5f, height * 0.7f });
    playButton.setScale({ 10.f, 10.f });

    // Football Initialization

    sf::Sprite football(texture3);

    sf::FloatRect footballBounds = football.getGlobalBounds();

    football.setOrigin({ footballBounds.size.x / 2.f, footballBounds.size.y / 2.f });
    Ball.ballCoords = sf::Vector2f({ width * 0.5f, height * 0.5f });
    football.setScale({ 1.f, 1.f });

    // Field Initialization

    sf::Sprite field(texture4);

    sf::FloatRect fieldBounds = field.getGlobalBounds();

    field.setOrigin({ fieldBounds.size.x / 2.f, fieldBounds.size.y / 2.f });
    field.setPosition({ width * 0.5f, height * 0.5f });
    field.setScale({ 10.f, 10.f });
    field.setColor(sf::Color(-50, -50, -50));

#pragma endregion

#pragma region Events

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close(); // Close Window
        }
        float dt = deltaClock.restart().asSeconds(); // Get Delta-Time

#pragma endregion

#pragma region Movement

        move(dt); // move the player (movement.h)

#pragma endregion

#pragma region Ball Handling

        int currentAction = 0; // Default action: nothing

        // Check for pickup (automatic or 'E' key)
        // If ball is moving, set pick-up distance higher to account for velocity changes
        float pickupDistance = !isBelow(Ball.velocity.x, 2.f) || !isBelow(Ball.velocity.y, 2.f) ? 50.f : 20.f;
        sf::Vector2f playerCenter = player.getGlobalBounds().getCenter();
        sf::Vector2f ballCenter = football.getGlobalBounds().getCenter(); 
        float distance = std::sqrt(std::pow(playerCenter.x - ballCenter.x, 2) + std::pow(playerCenter.y - ballCenter.y, 2));

        // Check for pass/shoot only if player has the ball and no other action is happening
        if (currentAction == 0 && Ball.isPickedUp && Ball.playerDribbling == 1) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                currentAction = 1; // Pass
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
                currentAction = 4; // Shoot
            }
        }

        if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) || (distance < pickupDistance && !Ball.isPickedUp)) && Data.cooldown < 1) {
            currentAction = 3; // Steal/Pickup
        }

        // Update the ball state based on the determined action
        updateBall(1, currentAction, 1, dt, window, 1);

#pragma endregion

#pragma region Button Handling

        if (playButton.clicked(window) && !(Data.plrVisible))
        {
            playButton.setColor(sf::Color(0, 0, 0, 0)); // Hide the playButton
            Data.plrVisible = true;
            player.setPosition({ width * 0.2f, height * 0.5f }); // Set player to start on left side of the field
            ai1.setPosition({ width * 0.8f, height * 0.5f }); // Set Bot to be across the field from the player
        }

#pragma endregion

#pragma region Bot Handling

        if (Data.plrVisible)
        {
            if (ai1.possesion)
            {
                ai1.target = Goals.goal1Coords;
            }
            if (ai1.wantsBall && !ai1.possesion)
            {
                ai1.target = Ball.ballCoords;
            }

            ai1.updateAI(dt, window, &football);

            if (!(ai1.action == ' '))
            {
                updateBall(2, actionGuide[ai1.action], 2, dt, window, 1, false, &ai1);
            }
        }

#pragma endregion

#pragma region Drawing Screen

        window.clear(sf::Color::Green);

        window.draw(field);

        // Set the Ball's position
        Ball.ballCoords += Ball.velocity;
        Ball.ballCoords = fixBounds(Ball.ballCoords, Ball.velocity, dt, screenWidth, screenHeight, true);
        football.setPosition(Ball.ballCoords);

        window.draw(football);

        if (Data.plrVisible == true)
        {

            sf::Vector2f newPlayerPos = fixBounds(player.getPosition(), Data.velocity, dt, screenWidth, screenHeight);

            directionFlip(&player, Data.facingDirection, texturePlayerLeft, texturePlayerRight);
            directionFlip(&ai1, Data.facingDirection, texturePlayerLeft, texturePlayerRight);

            player.setPosition(newPlayerPos);
            Data.playerCoords = player.getPosition();

            window.draw(ai1);
            window.draw(player);
        }

        window.draw(playButton);

        window.display();

        Data.cooldown -= 200 * dt;

#pragma endregion
    }
}