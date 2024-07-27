#pragma once
#include <glm/glm.hpp>
#include <raylib.h>

void initGame(const int width, const int height);
void gameLogic(float deltaTime);
void renderGame();
void closeGame();

