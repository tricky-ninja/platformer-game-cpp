#pragma once
#include <glm/glm.hpp>
#include <raylib.h>
#include <string>
#include <vector>

namespace Surface
{
	const unsigned NONE = 0;
	const unsigned LEFT = 0b1;
	const unsigned RIGHT = 0b10;
	const unsigned UP = 0b100;
	const unsigned DOWN = 0b1000;
	const unsigned FULL_BLOCK = 0b10000;
};

enum TileType
{
	TILE_NORMAL,
	TILE_DECORATION,
	TILE_ONEWAY
};

struct Tile
{
	glm::i64vec2 pos = {0,0};
	bool hasCollision=true;
	TileType type=TILE_NORMAL;
	size_t tilesetID=0;
	glm::i64vec2 tilesetPos = {2,1};

	void render();
};

struct Entity
{
	glm::vec2 pos = { 0,0 };
	glm::i64vec2 size = {50,75};
	glm::vec2 moveRemainder = { 0,0 };
	glm::vec2 vel = { 0,0 };
	unsigned collisionSurface = Surface::NONE;
	std::string texture="";

	bool isCollidingWithTile(const glm::vec2& tile);
	bool willCollideWithTile(const glm::vec2& tile, const glm::vec2& moveAmt);
	void render();
};

namespace Objects
{
	extern std::vector<Tile*> tiles;
	extern std::vector<Entity*> entities;

	void renderAll();

	size_t addTile();
	size_t addEntity();

	// Entity functions
	bool isColliding(size_t entityID);
	bool willCollide(size_t entityID, glm::vec2 moveAmt);
	void moveX(size_t entityID, float deltaTime);
	void moveY(size_t entityID, float deltaTime);
	void move(size_t entityID, float deltaTime);

	void freeObjects();
};