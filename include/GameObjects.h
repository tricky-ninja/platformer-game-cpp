#pragma once
#include <glm/glm.hpp>
#include <raylib.h>
#include <string>
#include <vector>

namespace Surface
{
	const int NONE = 0;
	const int LEFT = 0b1;
	const int RIGHT = 0b10;
	const int UP = 0b100;
	const int DOWN = 0b1000;
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
	std::string tileset="";
	glm::i64vec2 tilesetPos = {0,0};

	void render();
};

struct Entity
{
	glm::vec2 pos = { 0,0 };
	glm::i64vec2 size = {50,75};
	glm::vec2 moveRemainder = { 0,0 };
	glm::vec2 vel = { 0,0 };
	char collisionSurface = Surface::NONE;
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
	void moveX(size_t entityID);
	void moveY(size_t entityID);
	void move(size_t entityID);
};