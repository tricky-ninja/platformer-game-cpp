#include <glm/glm.hpp>
#include <raylib.h>
#include <chrono>
#include <iostream>

#include "gameLayer.h"

int main()
{
	const int width = 1280;
	const int height = 720;
	InitWindow(width, height, "Platformer");
	//SetTargetFPS(144);

	initGame(width, height);

	auto stop = std::chrono::high_resolution_clock::now();
	while (!WindowShouldClose())
	{

		// Delta time calculations
		auto start = std::chrono::high_resolution_clock::now();

		float deltaTime = (std::chrono::duration_cast<std::chrono::nanoseconds>(start - stop)).count() / 1000000000.0;
		stop = std::chrono::high_resolution_clock::now();

		float augmentedDeltaTime = deltaTime;
		if (augmentedDeltaTime > 1.f / 10) { augmentedDeltaTime = 1.f / 10; }
		augmentedDeltaTime *= 100;


		// Main game loop
		gameLogic(augmentedDeltaTime);
		BeginDrawing();
			ClearBackground(ORANGE);
			renderGame();
		EndDrawing();
	}
	closeGame();
	CloseWindow();
}