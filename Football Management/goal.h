#pragma once

#include <SFML/Graphics.hpp>
#include "ball.h"

struct goalData
{
	sf::Vector2f goal1Coords;
	sf::Vector2f goal2Coords;

	
};

void initiateGoals(float width, float height);

int updateGoal(sf::Sprite* bar1, sf::Sprite* bar2, sf::Sprite* football);

extern goalData Goals;