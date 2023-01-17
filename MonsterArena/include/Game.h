#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ECS/Vector2D.h>
#include <map>
#include <string>
#include <chrono>
#include <SDL_mixer.h>

class AssetManager;
class ColliderComponent;

class Game {
private:
	SDL_Window* window;
	Mix_Music* bgMusic;
	Mix_Chunk* soundEfect;
	std::map<std::string, SDL_Color> colors;
public:
	Game();
	~Game();

	void init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen, int dif);
	void handleEvents();
	void update();
	void render();
	void clean();

	bool checkIsRunning();

	void LoadColors();
	SDL_Color GetColor(std::string id);

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static bool startScreen;
	static bool hasRoundStarted;
	static SDL_Rect camera;
	static Vector2D  windowSize;
	static AssetManager* assets;
	static Vector2D playerPos;

	static std::chrono::time_point<std::chrono::system_clock> startRoundTime;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectile,
		groupEnemies,
		groupItems,
	};
};