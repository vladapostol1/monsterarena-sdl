#include <Map.h>
#include <Game.h>
#include <fstream>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern Manager manager;

Map::Map(std::string tID, int ms, int ts) : textureID(tID), mapScale(ms), tileSize(ts) {
	scaledSize = ms * ts;
}

Map::~Map(){}

void Map::AddTile(int srcX, int srcY, int xPos, int yPos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, textureID, tileSize, mapScale);
	tile.addGroup(Game::groupMap);
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tile;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(tile);
			srcY = atoi(&tile) * tileSize;
			mapFile.get(tile);
			srcX = atoi(&tile) * tileSize;
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
			mapFile.ignore();
		}
	}

	mapFile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(tile);
			if (tile == '1')
			{
				auto& tool(manager.addEntity());
				tool.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tool.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}
