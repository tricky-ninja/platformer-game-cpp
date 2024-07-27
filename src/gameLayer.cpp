#include "gameLayer.h"
#include <vector>
#include <iostream>
#include "Tile.h"
#include "Player.h"
#include "rlgl.h"
#include "raymath.h"

struct GameplayData
{
	bool isPaused;
	std::vector<Tile> tiles;
	Player player;
	glm::vec2 windowSize;
};

GameplayData data;

void initGame(const int width, const int height)
{
	data.windowSize.x = width;
	data.windowSize.y = height;

	data.isPaused = true;

	data.player.pos = glm::vec2(100, 200);
	data.player.size = glm::vec2(50, 75);

}

void gameLogic(float deltaTime)
{
	if (IsKeyPressed(KEY_F5)) data.isPaused = !data.isPaused;
#pragma region gameLogic

	if (!data.isPaused)
	{

		data.player.vel.y += 4;
		if (IsKeyDown(KEY_A))
		{
			data.player.vel.x = -10;
		}
		else if (IsKeyDown(KEY_D))
		{
			data.player.vel.x = 10;
		}
		if (IsKeyDown(KEY_SPACE) && (data.player.collisionSurface & DOWN))
		{
			data.player.vel.y = -35;
		}

		std::vector<glm::vec2> tiles_vec;
		for (auto& tile : data.tiles)
		{
			tiles_vec.push_back(tile.pos);
		}

		data.player.move(tiles_vec);
		data.player.vel.x -= 3;
		if (data.player.collisionSurface & DOWN) data.player.vel.y = 0;
		if (data.player.vel.x < 0) data.player.vel.x = 0;
	}

#pragma endregion

#pragma region levelEditor

	if (data.isPaused)
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Tile newTile;
			Vector2 mousePos = GetMousePosition();
			newTile.pos = glm::vec2((int)mousePos.x / 50 * 50, (int)mousePos.y / 50 * 50);

			bool found = false;
			for (auto& tile : data.tiles)
			{
				if (newTile.pos != tile.pos)  continue;
				found = true;
				break;
			}
			if (!found) { data.tiles.push_back(newTile); }

		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			for (size_t i = 0; i < data.tiles.size(); i++)
			{
				if (data.tiles[i].pos == glm::vec2(GetMouseX() / 50 * 50, GetMouseY() / 50 * 50))
				{
					data.tiles.erase(data.tiles.begin() + i);
					break;
				}
			}
		}
	}

#pragma endregion
}

void renderGame()
{
	// Game rendering
	for (auto& tile : data.tiles)
	{
		tile.render();
	}
	data.player.render();

	// Level editor rendering
	if (data.isPaused)
	{
		DrawRectangle(GetMouseX() / 50 * 50, GetMouseY() / 50 * 50, 50, 50, Color({ 255,0,0,100 }));
		rlPushMatrix();
		rlTranslatef(0, 25 * 50, 0);
		rlRotatef(90, 1, 0, 0);
		DrawGrid(100, 50);
		rlPopMatrix();
	}
}

void closeGame()
{

}