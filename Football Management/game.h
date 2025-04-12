#pragma once

#include <SFML/Graphics.hpp>

struct gameData
{
	sf::Vector2f playerCoords;

	sf::Vector2f velocity;

	float speed;

	bool plrVisible;
	float facingDirection;
};

extern gameData Data;