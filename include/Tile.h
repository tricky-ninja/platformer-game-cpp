#pragma once
#include <raylib.h>
#include <glm/glm.hpp>

struct Tile
{
	glm::vec2 pos;
	Color color=WHITE;

	void render();
};