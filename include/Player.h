#pragma once
#include <glm/glm.hpp>
#include <raylib.h>

struct Player
{
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec2 moveRemainder = { 0,0 };
	glm::vec2 vel = { 0,0 };
	char collisionSurface;

	bool isCollidingWithTile(const glm::vec2 &tile);
	bool isColliding(const std::vector<glm::vec2>& tiles);
	bool Player::willCollideWithTile(const glm::vec2& tile, const glm::vec2 futurePos);
	bool willCollide(const std::vector<glm::vec2> &tiles, const glm::vec2 &futurePos);

	void moveX(float amt, const std::vector<glm::vec2>& tiles);
	void moveY(float amt, const std::vector<glm::vec2>& tiles);
	void move(const std::vector<glm::vec2>& tiles);
	void render();
};


	const int NONE = 0;
	const int LEFT = 0b1;
	const int RIGHT = 0b10;
	const int UP = 0b100;
	const int DOWN = 0b1000;
