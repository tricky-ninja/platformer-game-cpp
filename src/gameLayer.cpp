#include "gameLayer.h"
#include <vector>
#include <iostream>
#include <GameObjects.h>
#include "config.h"
#include "AssetManager.h"
#include <Json.hpp>
#include <fstream>

using json = nlohmann::json;

struct GameplayData
{
	bool isPaused;
	size_t playerID;
	glm::i64vec2 windowSize;
	Camera2D camera;
	size_t mainTilesetID;
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

	data.isPaused = false;

	data.playerID = Objects::addEntity();
	Objects::entities[data.playerID]->pos = glm::vec2(0,0);
	Objects::entities[data.playerID]->size = glm::vec2(50, 75);

	data.camera.target = Vector2({0.f,0.f});
	data.camera.offset = Vector2({ width / 2.f, height/2.f });
	data.camera.rotation = 0.f;
	data.camera.zoom = 0.8f;

	data.mainTilesetID = Assets::addTexture(ASSETS_PATH "tileset.png");

#pragma region loadLevel
	std::ifstream file(ASSETS_PATH "level.json");
	if (!file)
	{
		std::cout << "File not found level.json\n";
		assert(false);
#if PRODUCTION_BUILD == 1
		exit(1);
#endif
	}
	json jsonObj;
	file >> jsonObj;

	for (const auto& tile : jsonObj["tiles"])
	{
		Tile* newTile = new Tile;
		newTile->pos = glm::i64vec2(tile["x"], tile["y"]);
		newTile->hasCollision = tile["collision"];
		newTile->type = tile["type"];
		newTile->tilesetID = tile["tileset"];
		newTile->tilesetPos = glm::i64vec2(tile["tilesetX"], tile["tilesetY"]);
		Objects::tiles.push_back(newTile);
	}
#pragma endregion
}

void resetGame()
{
	Objects::entities[data.playerID]->pos = glm::vec2(0,0);
	Objects::entities[data.playerID]->vel = glm::vec2(0, 0);
}

void gameLogic(float deltaTime)
{
	if (IsKeyPressed(KEY_F5)) data.isPaused = !data.isPaused;
#pragma region gameLogic

	if (!data.isPaused)
	{

		// Update camera - update y on landing, update x when player is near edges
		if ((GetWorldToScreen2D(Vector2({ Objects::entities[data.playerID]->pos.x, Objects::entities[data.playerID]->pos.y }), data.camera).x < (data.windowSize.x / 4)) || (GetWorldToScreen2D(Vector2({ Objects::entities[data.playerID]->pos.x, Objects::entities[data.playerID]->pos.y }), data.camera).x > (data.windowSize.x / 4 * 3))) { 
			data.camera.target.x -= (data.camera.target.x - Objects::entities[data.playerID]->pos.x) * 0.02 * deltaTime;
		}
		if ((Objects::entities[data.playerID]->collisionSurface & Surface::DOWN) && data.camera.target.y != Objects::entities[data.playerID]->pos.y)
		{
			data.camera.target.y -= (data.camera.target.y - Objects::entities[data.playerID]->pos.y) * 0.2 * deltaTime;
		}
		data.camera.offset = Vector2({ data.windowSize.x / 2.f, data.windowSize.y / 2.f });

		// Update player
		Objects::entities[data.playerID]->vel.y += 2 * deltaTime;
		if (IsKeyDown(KEY_A))
		{
			Objects::entities[data.playerID]->vel.x = -10;
		}
		else if (IsKeyDown(KEY_D))
		{
			Objects::entities[data.playerID]->vel.x = 10;
		}
		if (IsKeyDown(KEY_SPACE) && (Objects::entities[data.playerID]->collisionSurface & Surface::DOWN))
		{
			Objects::entities[data.playerID]->vel.y = -30;
		}

		Objects::move(data.playerID, deltaTime);
		Objects::entities[data.playerID]->vel.x *= 0.25;	// Friction
		if (Objects::entities[data.playerID]->collisionSurface & Surface::DOWN) Objects::entities[data.playerID]->vel.y = 0;
		
		// Game over
		if (GetWorldToScreen2D(Vector2({ Objects::entities[data.playerID]->pos.x, Objects::entities[data.playerID]->pos.y }), data.camera).y > data.windowSize.y + 200) resetGame();
	}

#pragma endregion

#pragma region levelEditor

	if (data.isPaused)
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Tile *newTile = new Tile;
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), data.camera);
			int adjustedX = (int)floor(mousePos.x / tileSize) * tileSize;
			int adjustedY = (int)floor(mousePos.y / tileSize) * tileSize;
			newTile->pos = glm::vec2(adjustedX, adjustedY);

			bool found = false;
			for (auto& tile : Objects::tiles)
			{
				if (newTile->pos != tile->pos)  continue;
				found = true;
				break;
			}
			if (!found) { Objects::tiles.push_back(newTile); }

		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), data.camera);
			int adjustedX = (int)floor(mousePos.x / tileSize) * tileSize;
			int adjustedY = (int)floor(mousePos.y / tileSize) * tileSize;
			for (size_t i = 0; i < Objects::tiles.size(); i++)
			{
				if (Objects::tiles[i]->pos == glm::i64vec2(adjustedX, adjustedY))
				{
					Objects::tiles.erase(Objects::tiles.begin() + i);
					break;
				}
			}
		}
#pragma region SaveLevel
		if (IsKeyPressed(KEY_Q))
		{
			json jsonObj;

			for (const Tile *tile : Objects::tiles)
			{
				jsonObj["tiles"].push_back({ {"x", tile->pos.x}, {"y", tile->pos.y}, {"collision", tile->hasCollision}, {"type", tile->type}, {"tileset", tile->tilesetID}, {"tilesetX", tile->tilesetPos.x}, {"tilesetY", tile->tilesetPos.y} });
			}
			std::ofstream file(ASSETS_PATH "level.txt");
			file << jsonObj.dump(4);
		}
#pragma endregion

		if (IsKeyDown(KEY_A)) data.camera.offset.x += 4 * deltaTime;
		if (IsKeyDown(KEY_D)) data.camera.offset.x -= 4 * deltaTime;
		if (IsKeyDown(KEY_W)) data.camera.offset.y += 4 * deltaTime;
		if (IsKeyDown(KEY_S)) data.camera.offset.y -= 4 * deltaTime;
		data.camera.zoom += ((float)GetMouseWheelMove() * 0.05f);
	}

#pragma endregion temporaray until an external level editor and loading system is developed
}

void renderGame()
{
	BeginMode2D(data.camera);
	Objects::renderAll();

	// Level editor rendering
	if (data.isPaused)
	{
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), data.camera);
		int adjustedX = (int)floor(mousePos.x / tileSize) * tileSize;
		int adjustedY = (int)floor(mousePos.y / tileSize) * tileSize;
		DrawRectangle(adjustedX, adjustedY, 50, 50, Color({ 150,100,100,100 }));
		EndMode2D();
		DrawText("Paused", 40, 40, 50, WHITE);
	}
	else
	{
		EndMode2D();
	}
	DrawFPS(40, 100);
}

void closeGame()
{
	Objects::freeObjects();
	std::cout << "Freed all resources. Closing game!\n";
}