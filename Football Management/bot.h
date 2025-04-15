#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

int aiInitialize();

extern std::map<char, int> actionGuide;

namespace sf
{
	class Bot : public Sprite
	{
	public:


		Vector2f velocity; // Speed for x and y
		Vector2f target; // Where to go? Just ball if wantsBall is true

		int team; // Which team you are on
		int cooldown; // Which team you are on

		float energy; // Amount of running left, reset when benched
		float maxSpeed; // How fast are you

		FloatRect bounds;
		
		std::string tmPos; // CM, GK, ST, ETC.
		char action; // T: Tackle, S: Shoot, L: Slide, P: Pass, R: Steal, ' ': None

		bool shoot; // Wants To Shoot
		bool wantsBall; // Never true if hasBall is true
		bool possesion; // Dribbling ball
		bool sprinting; // Sprinting

		void updateAI(float dt, sf::RenderWindow& window, sf::Sprite* football); // UpdateAI
		void changeAction(char newAction); // Change the bots action
		void sprintToggle();

		//
		// Constructors
		//

		explicit Bot(const Texture& texture); // No IntRect Creation
	private:
		bool ballVisionBlocked; // If everything stopped moving, and you ran straight, could you get the ball?
	};
}