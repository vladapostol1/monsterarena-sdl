#pragma once

#include "Components.h"
#include <SDL.h>
#include <TextureManager.h>
#include "Animation.h"
#include <AssetManager.h>

class SpriteComponent : public Component {
private:
	TransformComponent* transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	bool scaler = false;
	bool rotate = false;
	int frames = 0;
	int speed = 100;
	int rotationSpeed = 0;
	float angle = 0;

public:

	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;

	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, int x, int y, bool isScaled)
	{
		scaler = isScaled;
		destRect.h = x;
		destRect.w = y;
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 6, 100);
		Animation walk = Animation(1, 6, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);

		Play("Idle");

		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated, int animSteps)
	{
		animated = isAnimated;

		Animation walk = Animation(0, animSteps, 100);

		animations.emplace("Walk", walk);

		Play("Walk");

		setTex(id);
	}

	~SpriteComponent()
	{
	}

	void setTex(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
			srcRect.y = animIndex * transform->height;
		}


		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;

		if (!scaler)
		{
			destRect.w = transform->width * transform->scale;
			destRect.h = transform->height * transform->scale;
		}

		if (rotate)
		{
			angle += rotationSpeed;
		}
	}
	void draw() override
	{
			TextureManager::Draw(texture, srcRect, destRect, angle, spriteFlip);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

	void SetSrc(int srcX, int srcY)
	{
		srcRect.x = srcX;
		srcRect.y = srcY;
	}

	void ChangeRotation(bool rot, int dir = 0, int ang=0)
	{
		rotate = rot;
		rotationSpeed = dir;
		angle = ang;
	}

	bool GetRot()
	{
		return rotate;
	}
};