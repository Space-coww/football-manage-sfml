#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath> // For std::sqrt and std::pow
#include <CoreFoundation/CoreFoundation.h> // For macOS resource path
#include <cassert> // Include for assertions
#include <optional> // For SFML 3.0 event handling
#include <SFML/Graphics/Rect.hpp>


#pragma region headers

#include "game.h"
#include "button.h"
#include "movement.h"
#include "ball.h"
#include "goal.h"
#include "bot.h"

#pragma endregion

sf::Clock deltaClock;

// Helper function to get the resource path dynamically
std::string getResourcePath() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)) {
        std::cerr << "Error getting resource path!" << std::endl;
        exit(-1);
    }
    CFRelease(resourcesURL);
    return std::string(path) + "/";
}

sf::Vector2f fixBounds(sf::Vector2f currentPosition, sf::Vector2f velocity, float dt, int screenWidth, int screenHeight) {
    sf::Vector2f displacement = velocity * dt;
    sf::Vector2f newPosition = currentPosition + displacement;

    if (newPosition.x < 0) newPosition.x = 0;
    else if (newPosition.x > screenWidth) newPosition.x = screenWidth;

    if (newPosition.y < 0) newPosition.y = 0;
    else if (newPosition.y > screenHeight) newPosition.y = screenHeight;

    return newPosition;
}

bool isBelow(float number, float standard)
{
    return std::abs(number) < standard;
}

// Corrected intersects function to manually check for intersection
bool intersects(const sf::IntRect& a, const sf::IntRect& b) {
    return (a.position.x < b.position.x + b.size.x &&
            a.position.x + a.size.x > b.position.x &&
            a.position.y < b.position.y + b.size.y &&
            a.position.y + a.size.y > b.position.y);
}

// Remove duplicate or incorrect Ball/Data struct definitions if already defined in headers
// If not defined in ball.h, keep these minimal definitions here:
#ifndef BALL_STRUCT_DEFINED
#define BALL_STRUCT_DEFINED
struct Ball {
    static sf::Vector2f ballCoords;
    static sf::Vector2f velocity;
    static bool isPickedUp;
    static int playerDribbling;
};
sf::Vector2f Ball::ballCoords = {0.f, 0.f};
sf::Vector2f Ball::velocity = {0.f, 0.f};
bool Ball::isPickedUp = false;
int Ball::playerDribbling = 0;
#endif

#ifndef DATA_STRUCT_DEFINED
#define DATA_STRUCT_DEFINED
struct Data {
    static sf::Vector2f velocity;
    static sf::Vector2f playerCoords;
    static bool plrVisible;
    static float facingDirection;
};
sf::Vector2f Data::velocity = {0.f, 0.f};
sf::Vector2f Data::playerCoords = {0.f, 0.f};
bool Data::plrVisible = false;
float Data::facingDirection = 1.0f;
#endif

// Helper function to get the center of an sf::FloatRect
sf::Vector2f getCenter(const sf::FloatRect& rect) {
    return {
        rect.position.x + rect.size.x / 2.f,
        rect.position.y + rect.size.y / 2.f
    };
    
}

int main()
{
    std::cout << "Starting program..." << std::endl;

    // Window
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Footballs!", sf::State::Fullscreen); // Changed to Fullscreen
    std::cout << "Window created successfully." << std::endl;

    window.setFramerateLimit(60);
    window.setMouseCursorVisible(true);

    sf::Vector2u screenSize = sf::VideoMode::getFullscreenModes()[0].size;
    float screenWidth = static_cast<float>(screenSize.x);
    float screenHeight = static_cast<float>(screenSize.y);

    std::cout << "Screen size: " << screenWidth << "x" << screenHeight << std::endl;

    // Get the resource path
    std::string resourcePath = getResourcePath();

    // Load textures
    sf::Texture texture1;
    if (!texture1.loadFromFile(resourcePath + "player.png")) {
        std::cerr << "Error loading player.png!" << std::endl;
        return -1;
    }
    std::cout << "Loaded player.png successfully." << std::endl;

    sf::Texture texturePlayerLeft;
    if (!texturePlayerLeft.loadFromFile(resourcePath + "player2.png")) {
        std::cerr << "Error loading player2.png!" << std::endl;
        return -1;
    }

    sf::Texture texture2;
    if (!texture2.loadFromFile(resourcePath + "play.png")) {
        std::cerr << "Error loading play.png!" << std::endl;
        return -1;
    }

    sf::Texture texture3;
    if (!texture3.loadFromFile(resourcePath + "football.png")) {
        std::cerr << "Error loading football.png!" << std::endl;
        return -1;
    }

    sf::Texture texture4;
    if (!texture4.loadFromFile(resourcePath + "field.png")) {
        std::cerr << "Error loading field.png!" << std::endl;
        return -1;
    }

    // Width and Height of screen for reusability
    int width = window.getSize().x;
    int height = window.getSize().y;

#pragma region Sprite Initialization

    // Player Initialization
    sf::Sprite player(texture1);
    sf::FloatRect playerBounds = player.getGlobalBounds();
    sf::Vector2f playerSize = { playerBounds.size.x, playerBounds.size.y };
    player.setOrigin({ playerSize.x / 2.f, playerSize.y / 2.f });
    player.setScale({ 0.1f, 0.1f });

    // Play Button Initialization
    Button playButton(texture2);
    sf::FloatRect playButtonBounds = playButton.getGlobalBounds();

    // *** Assertion to check the type of playButtonBounds ***
    assert(typeid(playButtonBounds) == typeid(sf::FloatRect));

    playButton.setOrigin({ playButtonBounds.size.x / 2.f, playButtonBounds.size.y / 2.f });
    playButton.setPosition({ width * 0.5f, height * 0.7f });
    playButton.setScale({ 10.f, 10.f });

    // Football Initialization
    sf::Sprite football(texture3);
    sf::FloatRect footballBounds = football.getGlobalBounds();
    football.setOrigin({ footballBounds.size.x / 2.f, footballBounds.size.y / 2.f });
    Ball::ballCoords = sf::Vector2f({ width * 0.5f, height * 0.5f });
    football.setScale({ 1.f, 1.f });

    // Field Initialization
    sf::Sprite field(texture4);
    sf::FloatRect fieldBounds = field.getGlobalBounds();
    field.setOrigin({ fieldBounds.size.x / 2.f, fieldBounds.size.y / 2.f });
    field.setPosition({ width * 0.5f, height * 0.5f });
    field.setScale({ 10.f, 10.f });
    field.setColor(sf::Color(255, 255, 255)); // Set to full brightness (white tint)

#pragma endregion

#pragma region Events

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event && event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        float dt = deltaClock.restart().asSeconds();

#pragma endregion

#pragma region Movement

        move(dt); // Call the move function from movement.cpp to handle player movement

#pragma endregion

#pragma region Ball Handling

        int currentAction = 0; // Default action: nothing

        // Corrected ball handling logic
        float pickupDistance = 50.0f; // Adjust this distance as needed
        sf::Vector2f playerCenter = getCenter(player.getGlobalBounds());
        sf::Vector2f ballCenter = getCenter(football.getGlobalBounds());
        float distance = std::sqrt(std::pow(playerCenter.x - ballCenter.x, 2) + std::pow(playerCenter.y - ballCenter.y, 2));

        if ((distance < pickupDistance || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) && !Ball::isPickedUp) {
            currentAction = 3; // Steal/Pickup
            Ball::isPickedUp = true;
            Ball::playerDribbling = 1; // Player is now dribbling
        }

        // Check for pass/shoot only if player has the ball
        if (Ball::isPickedUp && Ball::playerDribbling == 1) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                currentAction = 1; // Pass
                Ball::isPickedUp = false; // Ball is no longer picked up
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
                currentAction = 4; // Shoot
                Ball::isPickedUp = false; // Ball is no longer picked up
            }
        }

        // Update the ball state based on the determined action
        updateBall(1, currentAction, 1, dt, window, 1);

#pragma endregion

#pragma region Button Handling

        if (playButton.clicked(window) && !(Data::plrVisible))
        {
            playButton.setColor(sf::Color(0, 0, 0, 0));
            Data::plrVisible = true;
            player.setPosition({ width * 0.2f, height * 0.5f });
        }

#pragma endregion

#pragma region Drawing Screen

        window.clear(sf::Color::Green);

        // Debugging: Check texture validity
        if (!texture1.getSize().x || !texture1.getSize().y) std::cerr << "Texture 1 is invalid!" << std::endl;
        if (!texture2.getSize().x || !texture2.getSize().y) std::cerr << "Texture 2 is invalid!" << std::endl;
        if (!texture3.getSize().x || !texture3.getSize().y) std::cerr << "Texture 3 is invalid!" << std::endl;
        if (!texture4.getSize().x || !texture4.getSize().y) std::cerr << "Texture 4 is invalid!" << std::endl;

        // Add this at the top of your file
        sf::Clock debugClock;

        // Inside the Drawing Screen section
        if (debugClock.getElapsedTime().asSeconds() >= 1.0f) { // Print every 1 second
            std::cout << "Ball Position: " << Ball::ballCoords.x << ", " << Ball::ballCoords.y << std::endl;
            std::cout << "Player Position: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;
            debugClock.restart(); // Reset the debug timer
        }
        
        window.draw(field);

        // Corrected Ball position update
        Ball::ballCoords += Ball::velocity * dt;
        Ball::ballCoords = fixBounds(Ball::ballCoords, Ball::velocity, dt, screenWidth, screenHeight);
        football.setPosition(Ball::ballCoords);

        window.draw(football);

        // Corrected player position update
        if (Data::plrVisible) {
            sf::Vector2f newPlayerPos = fixBounds(player.getPosition(), Data::velocity, dt, screenWidth, screenHeight);

            if (Data::facingDirection == -1.0f)
                player.setTexture(texturePlayerLeft);
            else
                player.setTexture(texture1);

            player.setPosition(newPlayerPos);
            Data::playerCoords = player.getPosition();

            window.draw(player);
        }

        window.draw(playButton);

        window.display();

#pragma endregion
    }
}