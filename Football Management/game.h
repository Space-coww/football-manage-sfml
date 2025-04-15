#pragma once

#include <SFML/Graphics.hpp>

struct gameData
{
	sf::Vector2f playerCoords;
	sf::Vector2f ai1Coords;

	sf::Vector2f velocity;
	sf::Vector2f ai1Velocity;

	float speed;

	int cooldown;

	bool plrVisible;
	float facingDirection;
	float aiFacingDirection;
};

extern gameData Data;