#pragma once
#include <map>
#include <string>
#include <TextureManager.h>
#include "ECS/ECS.h"
#include <SDL_ttf.h>

class AssetManager
{
private:
	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;

public:
	AssetManager(Manager* man);
	~AssetManager();

	void CreateProjectile(Vector2D pos, int range, int speed, int damage,Vector2D velocity, std:: string id);
	void CreateEnemie(Vector2D pos, int ammount, int regen, int dmg, int armr, int atSpd, int movSpd, std::string id, int exp, bool ranged = false);
	void CreateItem(Vector2D pos, std::string textureID, int weaponID, int level);
	void CreateItem(Vector2D pos, std::string textureID);
	void CreateWave(int ammount, int type, int dif);

	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);
};