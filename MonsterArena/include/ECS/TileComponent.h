#pragma once

#include "ECS.h"
#include <SDL.h>
#include <AssetManager.h>

class TileComponent : public Component {
public:

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;


	TileComponent() = default;

	~TileComponent()
	{
	}

	TileComponent(int srcX, int srcY, int xPos, int yPos, std::string id, int size, int scale){
		texture = Game::assets->GetTexture(id);

		position.x = xPos;
		position.y = yPos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = size;

		destRect.x = xPos;
		destRect.y = yPos;
		destRect.w = destRect.h = size * scale;
	}

	void update() override
	{
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, 0,SDL_FLIP_NONE);
	}
};