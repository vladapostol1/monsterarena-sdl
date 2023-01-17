#include <Game.h>
#include <fstream>
std::ifstream settings("res/settings.txt", std::ifstream::binary);

Game* game = nullptr;

int main(int argc, char* argv[])
{
	game = nullptr;
	game = new Game();
	int height = 600;
	int width = 800;
	int difficulty = 1;

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	std::string setName;
	int setVal;
	while (settings >> setName >> setVal)
	{
		if (setName == "height:")
			height = setVal;
		else if (setName == "width:")
			width = setVal;
		else if (setName == "difficulty:")
			difficulty = setVal;
	}

	game->init("Monster Arena", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, false, difficulty);

	while (game->checkIsRunning())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

return 0;
}