#pragma once
#include <vector>
#include <raylib.h>
#include <string>

namespace Assets
{
	extern std::vector<Texture2D*> gameTextures;
	size_t addTexture(const std::string& filename);
}
