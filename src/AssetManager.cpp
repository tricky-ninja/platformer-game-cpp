#pragma once
#include "AssetManager.h"
#include <cassert>
#include <iostream>

namespace Assets
{
	std::vector<Texture2D *> gameTextures;

	size_t addTexture(const std::string& filename)
	{	
		if (!FileExists(filename.c_str()))
		{
			std::cout << "File " << filename << " doesn't exist!\n";
			assert(false);
#if PRODUCTION_BUILD == 1
			exit(0);
#endif
		}
		Texture2D* t = new Texture2D();
		*t = LoadTexture(filename.c_str());
		gameTextures.push_back(t);
		return gameTextures.size() - 1;
	}
}