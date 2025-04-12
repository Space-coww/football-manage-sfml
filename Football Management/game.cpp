#include "game.h"

#include <SFML/Graphics.hpp>

gameData Data = {
	{ 0.f, 0.f }, // Player coords
	{ 0.f, 0.f }, // Player velocity
	175.f, // Max speed (Increased significantly)
	false, // Is the player visible
	1.0f // Player facing direction (1.0 = right, -1.0 = left)
};