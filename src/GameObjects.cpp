#include "GameObjects.h"
#include "config.h"
#include <iostream>
#include <AssetManager.h>

namespace Objects
{
	std::vector<Tile*> tiles;
	std::vector<Entity*> entities;
}

void Tile::render()
{
	if (tilesetID >= Assets::gameTextures.size())
	{
		DrawRectangle(this->pos.x, this->pos.y, tileSize, tileSize, WHITE);
	}
	else
	{
		size_t tileWidth = 8;
		size_t tileHeight = 8;

		Rectangle src = { (float)(tilesetPos.x * tileWidth), (float)(tilesetPos.y * tileHeight), (float)tileWidth, (float)tileHeight };
		Rectangle dst = { (float)pos.x + tileSize, (float)pos.y + tileSize, (float)tileSize, (float)tileSize };

		DrawTexturePro(*Assets::gameTextures[tilesetID], src, dst, Vector2({ dst.width, dst.height }), 0.f, WHITE);
	}
}

bool Entity::isCollidingWithTile(const glm::vec2& tile)
{
	return tile.x < pos.x + size.x / 2 && // Collision on Left of a and right of b
		tile.x + tileSize > pos.x - size.x / 2 && // Collision on Right of a and left of b
		tile.y < pos.y + size.y / 2 && // Collision on Bottom of a and Top of b
		tile.y + tileSize > pos.y - size.y / 2; // Collision on Top of a and Bottom of b
}

bool Entity::willCollideWithTile(const glm::vec2& tile, const glm::vec2& moveAmt)
{
	const glm::vec2 futurePos = pos + moveAmt;
	return tile.x < futurePos.x + size.x / 2 && // Collision on Left of a and right of b
		tile.x + tileSize > futurePos.x - size.x / 2 && // Collision on Right of a and left of b
		tile.y < futurePos.y + size.y / 2 && // Collision on Bottom of a and Top of b
		tile.y + tileSize > futurePos.y - size.y / 2; // Collision on Top of a and Bottom of b
}

void Entity::render()
{
	DrawRectangle(pos.x - size.x / 2, pos.y - size.y / 2, size.x, size.y, RED);
}

size_t Objects::addEntity()
{
	Entity* e = new Entity();
	entities.push_back(e);
	return entities.size() - 1;
}

bool Objects::isColliding(size_t entityID)
{
	assert(entities.size() > entityID);
	Entity* e = entities[entityID];

	for (auto tile : tiles)
	{
		if (!tile->hasCollision) continue;
		if (tile->type == TILE_ONEWAY)
		{
			if (e->pos.y + e->size.y / 2 > tile->pos.y) continue;
		}
		if (e->isCollidingWithTile(tile->pos)) return true;
	}

	return false;
}

bool Objects::willCollide(size_t entityID, glm::vec2 moveAmt)
{
	assert(entities.size() > entityID);
	Entity* e = entities[entityID];

	for (auto tile : tiles)
	{
		if (!tile->hasCollision) continue;
		if (tile->type == TILE_ONEWAY)
		{
			if (e->pos.y + e->size.y/2 > tile->pos.y) continue;
		}
		if (e->willCollideWithTile(tile->pos, moveAmt)) return true;
	}

	return false;
}

void Objects::moveX(size_t entityID, float deltaTime)
{
	assert(entities.size() > entityID);

	Entity* e = entities[entityID];
	float amt = e->vel.x;

	// We only move by whole numbers the remainder is stored,
	// it can be used in the next frame if the remainder adds up to a whole number
	e->moveRemainder.x += amt * deltaTime;
	int move = (int)e->moveRemainder.x / 1;
	if (!move) return;
	e->moveRemainder.x -= move;
	int sign = move / abs(move);

	// Move by 1 pixel and check for collisions
	while (move != 0)
	{
		if (!willCollide(entityID, glm::vec2(sign, 0)))
		{
			e->pos.x += sign;
			move -= sign;
			e->collisionSurface = Surface::NONE;
		}
		else
		{
			if (sign > 0) e->collisionSurface |= Surface::RIGHT;
			else e->collisionSurface |= Surface::LEFT;
			break;
		}
	}
}

void Objects::moveY(size_t entityID, float deltaTime)
{
	assert(entities.size() > entityID);

	Entity* e = entities[entityID];
	float amt = e->vel.y;

	// We only move by whole numbers the remainder is stored,
	// it can be used in the next frame if the remainder adds up to a whole number
	e->moveRemainder.y += amt * deltaTime;
	int move = (int)e->moveRemainder.y;
	if (!move) return;
	e->moveRemainder.y -= move;
	int sign = move / abs(move);

	// Move by 1 pixel and check for collisions
	while (move != 0)
	{
		if (!willCollide(entityID, glm::vec2(0, sign)))
		{
			e->pos.y += sign;
			move -= sign;
			e->collisionSurface = Surface::NONE;
		}
		else
		{
			if (sign > 0) e->collisionSurface |= Surface::DOWN;
			else e->collisionSurface |= Surface::UP;
			break;
		}
	}
}

void Objects::move(size_t entityID, float deltaTime)
{
	moveX(entityID, deltaTime);
	moveY(entityID, deltaTime);
}

void Objects::renderAll()
{
	for (auto tile : tiles)
	{
		tile->render();
	}

	for (auto entity : entities)
	{
		entity->render();
	}
}

size_t Objects::addTile()
{
	Tile* t = new Tile;
	tiles.push_back(t);
	return tiles.size() - 1;
}

void Objects::freeObjects()
{
	for (Tile* tile : tiles)
	{
		free(tile);
	}

	for (Entity* entity : entities)
	{
		free(entity);
	}

}
