#include <GameObject.h>
#include <TextureManager.h>

GameObject::GameObject(const char* texturesheet, int x, int y)
{
	renderer = Game::renderer;
	objTexture = TextureManager::LoadTexture(texturesheet);

	x_pos = x;
	y_pos = y;

	srcRect.h = 64;
	srcRect.w = 64;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = 64;
	destRect.y = 64;
	destRect.w = 64;
	destRect.h = 64;
}

GameObject::~GameObject()
{

}

void GameObject::Update()
{
}

void GameObject::Render()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
