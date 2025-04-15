#include <SFML/Graphics.hpp>

#include "goal.h"

goalData Goals = {
	{0.f, 0.f},
	{0.f, 0.f}
};

int updateGoal(sf::Sprite* bar1, sf::Sprite* bar2, sf::Sprite* football)
{

}

void initiateGoals(float width, float height)
{
	Goals.goal1Coords = sf::Vector2f{ 0.f, height / 2.f };
	Goals.goal2Coords = sf::Vector2f{ width, height / 2.f };
}