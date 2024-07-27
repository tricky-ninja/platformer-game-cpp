#include "Player.h"

#include <iostream>

bool Player::isCollidingWithTile(const glm::vec2& tile)
{
	return tile.x < pos.x + size.x / 2 && // Collision on Left of a and right of b
		tile.x + 50 > pos.x - size.x / 2 && // Collision on Right of a and left of b
		tile.y < pos.y + size.y / 2 && // Collision on Bottom of a and Top of b
		tile.y + 50 > pos.y - size.y / 2; // Collision on Top of a and Bottom of b
}

bool Player::willCollideWithTile(const glm::vec2& tile, const glm::vec2 futurePos)
{
	return tile.x < futurePos.x + size.x / 2 && // Collision on Left of a and right of b
		tile.x + 50 > futurePos.x - size.x / 2 && // Collision on Right of a and left of b
		tile.y < futurePos.y + size.y / 2 && // Collision on Bottom of a and Top of b
		tile.y + 50 > futurePos.y - size.y / 2; // Collision on Top of a and Bottom of b
}


bool Player::isColliding(const std::vector<glm::vec2> &tiles)
{
	for (auto& tile : tiles)
	{
		if (isCollidingWithTile(tile)) return true;
	}
	return false;
}

bool Player::willCollide(const std::vector<glm::vec2>& tiles, const glm::vec2& futurePos)
{
	for (auto& tile : tiles)
	{
		if (willCollideWithTile(tile, futurePos)) return true;
	}
	return false;
}

void Player::moveX(float amt, const std::vector<glm::vec2>& tiles)
{
	moveRemainder.x += amt;
	int move = (int)moveRemainder.x / 1;
	if (!move) return;
	moveRemainder.x -= move;
	int sign = move / abs(move);

	while (move != 0)
	{
		if (!willCollide(tiles, pos + glm::vec2(sign, 0)))
		{
			pos.x += sign;
			move -= sign;
			collisionSurface = NONE;
		}
		else
		{
			if (sign > 0) collisionSurface |= RIGHT;
			else collisionSurface |= LEFT;
			break;
		}
	}
}

void Player::moveY(float amt, const std::vector<glm::vec2>& tiles)
{
	moveRemainder.y += amt;
	int move = (int)moveRemainder.y / 1;
	if (!move) return;
	moveRemainder.y -= move;
	int sign = move / abs(move);
	while (move != 0)
	{
		if (!willCollide(tiles, pos + glm::vec2(0, sign)))
		{
			pos.y += sign;
			move -= sign;
			collisionSurface = NONE;
		}
		else
		{
			if (sign > 0) collisionSurface |= DOWN;
			else collisionSurface |= UP;
			break;
		}
	}
}

void Player::move(const std::vector<glm::vec2>& tiles)
{
	moveX(vel.x, tiles);
	moveY(vel.y, tiles);
}

void Player::render()
{
	DrawRectangle(pos.x - size.x / 2, pos.y - size.y / 2, size.x, size.y, RED);
}
