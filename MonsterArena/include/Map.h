#pragma once
#include <string>

class Map{
public:
	Map(std::string tID, int ms, int ts);
	~Map();

	void AddTile(int srcX, int srcY, int xPos, int yPos);
	void LoadMap(std::string path, int sizeX, int sizeY);
private:
	std::string textureID;
	int mapScale;
	int tileSize;
	int scaledSize;
};