#pragma once
#include <ECS/ECS.h>

class Button : public Component
{
private:
	SDL_Rect buttonRect;
	SDL_Color backgroundColor;
public:
	Button(Vector2D pos, Vector2D size, SDL_Color bgColor) : 
		backgroundColor(bgColor)
	{
		buttonRect.x = pos.x;
		buttonRect.y = pos.y;
		buttonRect.w = size.x;
		buttonRect.h = size.y;
	}
	~Button()
	{}

	void draw()
	{
		SDL_SetRenderDrawColor(Game::renderer, backgroundColor.r, backgroundColor.b, backgroundColor.g, 255);
		SDL_RenderDrawRect(Game::renderer, &buttonRect);
		SDL_RenderFillRect(Game::renderer, &buttonRect);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
	}

	void changeSize(int width, int percent)
	{
		if (width * percent < 0)
			buttonRect.w = 0;
		else
			buttonRect.w = width * percent / 100;
	}

	void changeColor(SDL_Color bg)
	{
		backgroundColor = bg;
	}

	bool isHitting(int xMouse, int yMouse)
	{
		if ((xMouse >= buttonRect.x) && (xMouse <= buttonRect.x + buttonRect.w)
			&& (yMouse >= buttonRect.y) && (yMouse <= buttonRect.y + buttonRect.h))
			return true;
		return false;
	}
};

class Outline : public Component
{
private:
	SDL_Rect outlineRect;
	SDL_Color backgroundColor;
public:
	Outline(Vector2D pos, Vector2D size, SDL_Color bgColor) :
		backgroundColor(bgColor)
	{
		outlineRect.x = pos.x;
		outlineRect.y = pos.y;
		outlineRect.w = size.x;
		outlineRect.h = size.y;
	}
	~Outline()
	{}

	void draw()
	{
		SDL_SetRenderDrawColor(Game::renderer, backgroundColor.r, backgroundColor.b, backgroundColor.g, 255);
		SDL_RenderDrawRect(Game::renderer, &outlineRect);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
	}
};
