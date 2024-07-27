#include <glm/glm.hpp>
#include <raylib.h>
#include <chrono>

#include "gameLayer.h"

int main()
{
	const int width = 1000;
	const int height = 900;
	InitWindow(width, height, "Platformer");
	SetTargetFPS(60);

	initGame(width, height);
	auto stop = std::chrono::high_resolution_clock::now();
	while (!WindowShouldClose())
	{
		auto start = std::chrono::high_resolution_clock::now();

		float deltaTime = (std::chrono::duration_cast<std::chrono::nanoseconds>(start - stop)).count() / 1000000000.0;
		stop = std::chrono::high_resolution_clock::now();

		float augmentedDeltaTime = deltaTime;
		if (augmentedDeltaTime > 1.f / 10) { augmentedDeltaTime = 1.f / 10; }

		gameLogic(augmentedDeltaTime);
		BeginDrawing();
		ClearBackground(BLACK);
		renderGame();
		EndDrawing();
	}
	CloseWindow();
}