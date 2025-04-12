#include "game.h"

#include <SFML/Graphics.hpp>

gameData Data = {
	{ 0.f, 0.f }, // Player coords
	{ 0.f, 0.f }, // Player velocity
	{ 0.f, 0.f }, // ai1 coords
	{ 0.f, 0.f }, // ai1 velocity
	175.f, // Max speed (Increased significantly)
	200, // Max speed (Increased significantly)
	false, // Is the player visible
	1.0f, // Player facing direction (1.0 = right, -1.0 = left)
	1.0f // Player facing direction (1.0 = right, -1.0 = left)
};