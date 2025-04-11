#pragma once

#include <SFML\Graphics.hpp>

struct ballData
{
	sf::Vector2f ballCoords;
	sf::Vector2f velocity;

	bool isPickedUp;

	int playerDribbling;

	float stealTimer;
};

extern ballData Ball;

void updateBall(int touchingPlayer, int action, int playerActing, float dt, sf::RenderWindow& window, int team);