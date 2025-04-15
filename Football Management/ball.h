#pragma once

#include <SFML\Graphics.hpp>
#include "bot.h"

void updateBall(int touchingPlayer, int action, int playerActing, float dt, sf::RenderWindow& window, int team, bool friction = false, sf::Bot* bot = nullptr);

struct ballData
{
	sf::Vector2f ballCoords;
	sf::Vector2f velocity;

	bool isPickedUp;

	int playerDribbling;

	float stealTimer;
};

extern ballData Ball;