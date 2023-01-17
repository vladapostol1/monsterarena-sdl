#include "AssetManager.h"
#include "ECS/Components.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::CreateProjectile(Vector2D pos, int range, int speed, int damage, Vector2D velocity, std::string id)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>("projectile", true, 3);
	projectile.addComponent<ProjectileComponent>(range, speed, damage, velocity);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectile);
}

void AssetManager::CreateEnemie(Vector2D pos, int ammount, int regen, int dmg, int armr, int atSpd, int movSpd, std::string id, int exp, bool ranged)
{
	auto& enemie(manager->addEntity());
	int spriteScaleX, spriteScaleY;
	spriteScaleX = 64;
	spriteScaleY = 64;
	if (id.compare("1"))
	{
		spriteScaleX = 40;
		spriteScaleY = 48;
	}
	else if (id.compare("2")) {
		spriteScaleX = 32;
		spriteScaleY = 48;
	}
	enemie.addComponent<TransformComponent>(pos.x, pos.y, spriteScaleY, spriteScaleX, 1);
	enemie.addComponent<SpriteComponent>(id, true, 2);
	enemie.addComponent<Stats>(ammount, regen, dmg, armr, atSpd, movSpd, exp, ranged);
	enemie.addComponent<Enemie>();
	enemie.addComponent<ColliderComponent>("enemie");
	enemie.addGroup(Game::groupEnemies);
}

void AssetManager::CreateItem(Vector2D pos, std::string textureID, int weaponID, int level)
{
	auto& item(manager->addEntity());
	item.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	item.addComponent<SpriteComponent>(textureID, false);
	item.addComponent<ColliderComponent>("item");
	item.addComponent<Item>(weaponID, level);
	item.addGroup(Game::groupItems);
}

void AssetManager::CreateItem(Vector2D pos, std::string textureID)
{
	auto& item(manager->addEntity());
	item.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	item.addComponent<SpriteComponent>(textureID, false);
	item.addComponent<ColliderComponent>("item");
	item.addComponent<Item>();
	item.addGroup(Game::groupItems);
}

void AssetManager::CreateWave(int ammount, int type, int dif)
{
	Vector2D tempPos(rand() % 1100 + 100, rand() % 1300 + 100);
	int offsetX = 50;
	int offsetY = 20;
	if (tempPos.x < 400)
		offsetX *= -1;
	if (tempPos.y < 400)
		offsetY *= -1;
	while (ammount > 0)
	{
		switch (type)
		{
		case 1:
			CreateEnemie(tempPos, 5 * dif, 0, 2 * dif, 0, 5, 2, "e1", 5);
			break;
		case 2:
			CreateEnemie(tempPos, 3 * dif, 0, 2 * dif, 0, 7, 1, "e2", 8, true);
			break;
		case 3:
			int x = 1 * dif / 2;
			x = x + 0.5 - (x < 0);
			CreateEnemie(tempPos, 10 * dif, 0, 5 * dif, 3 * dif, 3, int(x), "e3", 10);
			break;
		}
		ammount--;
		tempPos.x -= offsetX;
		tempPos.y -= offsetY;
	}
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}


