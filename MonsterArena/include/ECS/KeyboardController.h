#pragma once

#include <Game.h>
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN && Game::isRunning && !Game::startScreen)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->Play("Walk");
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->Play("Walk");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->Play("Walk");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->Play("Walk");
				break;
			default:
				break;
			}
		}
		if (Game::event.type == SDL_KEYUP && Game::isRunning && !Game::startScreen)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->Play("Idle");
				if (Game::event.type == SDL_KEYDOWN && Game::event.key.keysym.sym == SDLK_s)
				{
					transform->velocity.y = 1;
					sprite->Play("Walk");
				}
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->Play("Idle");
				if (Game::event.type == SDL_KEYDOWN && Game::event.key.keysym.sym == SDLK_w)
				{
					transform->velocity.y = -1;
					sprite->Play("Walk");
				}
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->Play("Idle");
				sprite->spriteFlip = SDL_FLIP_NONE;
				if (Game::event.type == SDL_KEYDOWN && Game::event.key.keysym.sym == SDLK_d)
				{
					transform->velocity.x = 1;
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					sprite->Play("Walk");
				}
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->Play("Idle");
				if (Game::event.type == SDL_KEYDOWN && Game::event.key.keysym.sym == SDLK_a)
				{
					transform->velocity.y = -1;
					sprite->Play("Walk");
				}
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			default:
				break;
			}
		}
		if (Game::event.type == SDL_MOUSEBUTTONDOWN && Game::startScreen)
		{
			int xMouse, yMouse;
			if (SDL_BUTTON(SDL_GetMouseState(&xMouse, &yMouse)) == SDL_BUTTON_LEFT)
			{
				if ((xMouse >= (Game::windowSize.x / 2 - 100)) && (xMouse <= (Game::windowSize.x / 2 + 100))
					&& (yMouse >= Game::windowSize.y - 200) && (yMouse <= Game::windowSize.y - 158))
				{
					Game::startScreen = false;
					Game::startRoundTime = std::chrono::system_clock::now();
					Game::hasRoundStarted = true;
				}
				if ((xMouse >= (Game::windowSize.x / 2 - 100)) && (xMouse <= (Game::windowSize.x / 2 + 100))
					&& (yMouse >= Game::windowSize.y - 140) && (yMouse <= Game::windowSize.y - 98))
					Game::isRunning = false;
			}
		}
	}
};