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
	Camera2D camera;
};

GameplayData data;

float easeInSine(float x)
{
	return 1 - cos((x * PI) / 2);
}

void initGame(const int width, const int height)
{
	data.windowSize.x = width;
	data.windowSize.y = height;

	data.isPaused = true;

	data.player.pos = glm::vec2(100, 200);
	data.player.size = glm::vec2(50, 75);

	data.camera.target = Vector2({ width / 2.f, height / 2.f });
	data.camera.offset = Vector2({ width / 2.f, height / 2.f });
	data.camera.rotation = 0.f;
	data.camera.zoom = 1.0f;
}

void resetGame()
{
	data.player.pos = glm::vec2(100, 200);
	data.player.vel = glm::vec2(0, -40);
}

void gameLogic(float deltaTime, unsigned short frame)
{
	if (IsKeyPressed(KEY_F5)) data.isPaused = !data.isPaused;
#pragma region gameLogic

	if (!data.isPaused)
	{
		if ((GetWorldToScreen2D(Vector2({ data.player.pos.x, data.player.pos.y }), data.camera).x < (data.windowSize.x / 4)) || (GetWorldToScreen2D(Vector2({ data.player.pos.x, data.player.pos.y }), data.camera).x > (data.windowSize.x / 4 * 3))) { 
			data.camera.target.x -= (data.camera.target.x - data.player.pos.x) * 0.02;
		}
		if ((data.player.collisionSurface & DOWN) && data.camera.target.y != data.player.pos.y)
		{
			data.camera.target.y -= (data.camera.target.y - data.player.pos.y) * 0.2;
		}

		data.camera.offset = Vector2({ data.windowSize.x / 2.f, data.windowSize.y / 2.f });

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
		if (GetWorldToScreen2D(Vector2({ data.player.pos.x, data.player.pos.y }), data.camera).y > data.windowSize.y + 200) resetGame();
	}

#pragma endregion

#pragma region levelEditor

	if (data.isPaused)
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Tile newTile;
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), data.camera);
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
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), data.camera);
			for (size_t i = 0; i < data.tiles.size(); i++)
			{
				if (data.tiles[i].pos == glm::vec2((int)mousePos.x / 50 * 50, (int)mousePos.y / 50 * 50))
				{
					data.tiles.erase(data.tiles.begin() + i);
					break;
				}
			}
		}

		if (IsKeyDown(KEY_A)) data.camera.offset.x += 4;
		if (IsKeyDown(KEY_D)) data.camera.offset.x -= 4;
		if (IsKeyDown(KEY_W)) data.camera.offset.y += 4;
		if (IsKeyDown(KEY_S)) data.camera.offset.y -= 4;
		data.camera.zoom += ((float)GetMouseWheelMove() * 0.05f);
	}

#pragma endregion
}

void renderGame(unsigned short frame)
{
	BeginMode2D(data.camera);
	for (auto& tile : data.tiles)
	{
		tile.render();
	}
	data.player.render();

	// Level editor rendering
	if (data.isPaused)
	{
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), data.camera);
		DrawRectangle((int)mousePos.x / 50 * 50, (int)mousePos.y / 50 * 50, 50, 50, Color({ 150,100,100,100 }));
	}
	EndMode2D();
}

void closeGame()
{

}