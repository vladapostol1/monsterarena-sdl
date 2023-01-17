#include <Game.h>
#include <TextureManager.h>
#include <Map.h>
#include <ECS/Components.h>
#include <Collision.h>
#include "AssetManager.h"
#include <sstream>

bool DEBUG_MODE = false;

Map* map;
Manager manager;

bool Game::isRunning = false;
bool Game::hasRoundStarted = false;
bool Game::startScreen = true;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Vector2D Game::windowSize = Vector2D(800, 600);

SDL_Rect Game::camera = { 0, 0, 800,600 };

AssetManager* Game::assets = new AssetManager(&manager);

std::chrono::time_point<std::chrono::system_clock> Game::startRoundTime;

auto& player(manager.addEntity());
auto& health(manager.addEntity());
auto& expbar(manager.addEntity());
auto& money(manager.addEntity());
auto& roundCounter(manager.addEntity());
auto& timeCounter(manager.addEntity());

auto& button1(manager.addEntity());
auto& button2(manager.addEntity());
auto& loadscreen(manager.addEntity());

auto& roundscreen(manager.addEntity());
auto& statusBtn1(manager.addEntity());
auto& statusBtn2(manager.addEntity());
auto& statusBtn3(manager.addEntity());
auto& statusBtn4(manager.addEntity());
auto& statusBtn5(manager.addEntity());
auto& statusBtn6(manager.addEntity());
auto& shopButtons(manager.addEntity());
auto& points(manager.addEntity());

int localScale = 2;
int roundTime = 40;
int currentRound = 1;
bool enemySpawned = false;
int spawnRate = 1;
Vector2D Game::playerPos = Vector2D(300, 300);

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen, int dif)
{
	Uint32 flags = 0;

	windowSize.x = static_cast<int>(width);
	windowSize.y = static_cast<int>(height);
	camera.w = windowSize.x;
	camera.h = windowSize.y;
	srand(time(NULL));

	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	if (DEBUG_MODE == true)
	{
		roundTime = 10000;
		currentRound = 0;
	}

	if (!SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO))
	{
		std::cout << "SDL has been initialized succesfully!...\n";

		window = SDL_CreateWindow(title, x_pos, y_pos, width, height, flags);

		if (window)
			std::cout << "Window was created!\n";
		else
			throw std::runtime_error("Window was not created!\n");


		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			std::cout << "Renderer was created!\n";
		}
		else
			throw std::runtime_error("Renderer was not created!\n");

		isRunning = true;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Error : " << Mix_GetError() << std::endl;
	}

	if (dif > 1)
	{
		spawnRate = dif;
	}

	LoadColors();

	//TEXURES
	assets->AddTexture("map", "res/tiles/tiles-list.png");
	assets->AddTexture("start", "res/start-screen.png");
	assets->AddTexture("player", "res/player/player-anim.png");
	assets->AddTexture("projectile", "res/projectile.png");
	assets->AddTexture("e1", "res/e1.png");
	assets->AddTexture("e2", "res/e2.png");
	assets->AddTexture("e3", "res/e3.png");
	assets->AddTexture("weapons", "res/player/weapons.png");

	//FONTS
	assets->AddFont("default", "res/font.fon", 16);
	assets->AddFont("default_32", "res/font.fon", 32);
	assets->AddFont("default_64", "res/font.fon", 64);

	//MAP
	map = new Map("map", 2, 32);
	map->LoadMap("res/map_20x20.map", 20, 20);

	//MUSIC
	//bgMusic = Mix_LoadMUS("res/sounds/bg-music.mp3");
	soundEfect = Mix_LoadWAV("res/sounds/slash.wav");
	//Mix_PlayMusic(bgMusic, -1);

	//PLAYER
	player.addComponent<Stats>(10, 0, 3, 0, 5, 1);
	player.addComponent<TransformComponent>(windowSize.x / 2 - 32, windowSize.y / 2 - 32, 50,32,1);
	player.addComponent<Level>();
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addComponent<PlayerInventory>();
	player.addGroup(groupPlayers);
	assets->CreateItem(Vector2D(200, 200), "weapons");
	for (auto& i : manager.getGroup(Game::groupItems))
	{
		player.getComponent<PlayerInventory>().EquipLeft(*i);
	}

	//START SCREEN
	loadscreen.addComponent<TransformComponent>(0, 0, 1080, 1920, 1);
	loadscreen.addComponent<SpriteComponent>("start", windowSize.y,windowSize.x,true);
	button1.addComponent<Button>(Vector2D(windowSize.x / 2 - 100, windowSize.y - 200), Vector2D(200, 42), GetColor("white"));
	button1.addComponent<UILabel>(windowSize.x / 2 - 20, windowSize.y - 190, "Play", "default_64", colors["black"]);
	button2.addComponent<Button>(Vector2D(windowSize.x / 2 - 100, windowSize.y - 140), Vector2D(200, 42), GetColor("white"));
	button2.addComponent<UILabel>(windowSize.x / 2 - 20, windowSize.y - 130, "Quit", "default_64", colors["black"]);

	//IN-GAME GUI
	health.addComponent<Button>(Vector2D(10, 10), Vector2D(150, 25), GetColor("red"));
	health.addComponent<Outline>(Vector2D(9, 10), Vector2D(151, 25), GetColor("white"));
	health.addComponent<UILabel>(12, 12, "Test", "default", colors["white"]);
	expbar.addComponent<Button>(Vector2D(10, 50), Vector2D(0, 25), GetColor("green"));
	expbar.addComponent<Outline>(Vector2D(9, 50), Vector2D(151, 25), GetColor("white"));
	expbar.addComponent<UILabel>(12, 52, "Lvl. 1", "default", colors["white"]);
	money.addComponent<UILabel>(12, 92, "Gold: 0", "default", colors["white"]);
	roundCounter.addComponent<UILabel>(windowSize.x/2 - 32, 12, "Round 1", "default", colors["white"]);
	timeCounter.addComponent<UILabel>(windowSize.x / 2 - 8, 40, "60", "default_64", colors["white"]);

	//ROUND SCREEN
	roundscreen.addComponent<Button>(Vector2D(0, 0), Vector2D(windowSize.x, windowSize.y), GetColor("transparent-grey"));
	roundscreen.addComponent<Button>(Vector2D(50, 50), Vector2D(windowSize.x * 3 / 4 - 100, windowSize.y * 3 / 4 - 100), GetColor("transparent-black"));
	roundscreen.addComponent<Button>(Vector2D(windowSize.x * 3 / 4, 50), Vector2D(windowSize.x * 1 / 4 - 50, windowSize.y * 3 / 4 - 100), GetColor("transparent-black"));
	roundscreen.addComponent<Button>(Vector2D(windowSize.x / 2 - 150, windowSize.y - 100), Vector2D(300, 50), GetColor("white"));
	roundscreen.addComponent<UILabel>(windowSize.x / 2 - 50, windowSize.y - 85, "Next Round", "default_64", colors["black"]);
	statusBtn1.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 60, "Max HP", "default", colors["white"]);
	statusBtn1.addComponent<Button>(Vector2D(windowSize.x -75 , 60), Vector2D(20, 20), GetColor("white"));
	statusBtn1.addComponent<UILabel>(windowSize.x - 70, 60, "+", "default", colors["black"]);
	statusBtn1.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 80, "10", "default", colors["white"]);
	statusBtn2.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 110, "Regen", "default", colors["white"]);
	statusBtn2.addComponent<Button>(Vector2D(windowSize.x - 75, 110), Vector2D(20, 20), GetColor("white"));
	statusBtn2.addComponent<UILabel>(windowSize.x - 70, 110, "+", "default", colors["black"]);
	statusBtn2.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 130, "0", "default", colors["white"]);
	statusBtn3.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 160, "Damage", "default", colors["white"]);
	statusBtn3.addComponent<Button>(Vector2D(windowSize.x - 75, 160), Vector2D(20, 20), GetColor("white"));
	statusBtn3.addComponent<UILabel>(windowSize.x - 70, 160, "+", "default", colors["black"]);
	statusBtn3.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 180, "3", "default", colors["white"]);
	statusBtn4.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 210, "Armor", "default", colors["white"]);
	statusBtn4.addComponent<Button>(Vector2D(windowSize.x - 75, 210), Vector2D(20, 20), GetColor("white"));
	statusBtn4.addComponent<UILabel>(windowSize.x - 70, 210, "+", "default", colors["black"]);
	statusBtn4.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 230, "0", "default", colors["white"]);
	statusBtn5.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 260, "Atk Speed", "default", colors["white"]);
	statusBtn5.addComponent<Button>(Vector2D(windowSize.x - 75, 260), Vector2D(20, 20), GetColor("white"));
	statusBtn5.addComponent<UILabel>(windowSize.x - 70, 260, "+", "default", colors["black"]);
	statusBtn5.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 280, "5", "default", colors["white"]);
	statusBtn6.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 310, "Speed", "default", colors["white"]);
	statusBtn6.addComponent<Button>(Vector2D(windowSize.x - 75, 310), Vector2D(20, 20), GetColor("white"));
	statusBtn6.addComponent<UILabel>(windowSize.x - 70, 310, "+", "default", colors["black"]);
	statusBtn6.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, 330, "1", "default", colors["white"]);
	points.addComponent<UILabel>(windowSize.x * 3 / 4 + 5, windowSize.y * 3 / 4 - 75, "Points: 0", "default", colors["white"]);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectile));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& items(manager.getGroup(Game::groupItems));


void Game::LoadColors()
{
	colors.emplace("white", SDL_Color{ 255, 255, 255, 255 });
	colors.emplace("black", SDL_Color{ 0, 0, 0, 255 });
	colors.emplace("grey", SDL_Color{ 200, 200, 200, 255 });
	colors.emplace("green", SDL_Color{ 0, 168, 58, 255 });
	colors.emplace("red", SDL_Color{ 179, 23, 41, 255 });
	colors.emplace("yellow", SDL_Color{ 255, 208, 0, 255 });
	colors.emplace("transparent-grey", SDL_Color{ 200, 200, 200, 150 });
	colors.emplace("transparent-black", SDL_Color{ 50, 50, 50, 200 });
}

SDL_Color Game::GetColor(std::string id)
{
	return colors[id];
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	int playerHealth = player.getComponent<Stats>().GetHealth();
	int maxHealth = player.getComponent<Stats>().GetMaxHealth();
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	playerPos = player.getComponent<TransformComponent>().position;

	std::stringstream ss;
	ss << playerHealth << "/" << maxHealth;
	health.getComponent<UILabel>().SetLabelText(ss.str(), "default");
	health.getComponent<Button>().changeSize(150, int(playerHealth * 100 / maxHealth));

	ss.str("");
	ss << "Lvl. " << player.getComponent<Level>().ReturnLevel();
	expbar.getComponent<UILabel>().SetLabelText(ss.str(), "default");
	expbar.getComponent<Button>().changeSize(150, int(player.getComponent<Level>().ReturnExp() * 100 / player.getComponent<Level>().ExpNeeded()));

	ss.str("");
	ss << "Gold: " << player.getComponent<Level>().ReturnGold();
	money.getComponent<UILabel>().SetLabelText(ss.str(), "default");

	ss.str("");
	std::chrono::duration<double> elapsed_seconds = startRoundTime - std::chrono::system_clock::now();
	ss << roundTime + int(elapsed_seconds.count()) ;
	timeCounter.getComponent<UILabel>().SetLabelText(ss.str(), "default_64");


	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& p : projectiles)
	{
		if (Collision::AABB(playerCol, p->getComponent<ColliderComponent>().collider))
		{
			player.getComponent<Stats>().TakeDamage(p->getComponent<ProjectileComponent>().GetDamage());
			p->destroy();
		}
	}

	for (auto& e : enemies)
	{
		if (Collision::AABB(playerCol, e->getComponent<ColliderComponent>().collider))
		{
			player.getComponent<Stats>().TakeDamage(e->getComponent<Enemie>().DoDamage());
			e->getComponent<Enemie>().KnockBack();
		}
		Vector2D enemPos = e->getComponent<TransformComponent>().position;
		if (player.getComponent<PlayerInventory>().HasLeft())
		{
			if (sqrt(pow((enemPos.x - (playerPos.x - 10)), 2) + pow((enemPos.y - playerPos.y - 10), 2)) <=
				player.getComponent<PlayerInventory>().GetRange("left") * 10)
			{
				Mix_PlayChannel(1, soundEfect, 0);
				e->getComponent<Stats>().TakeDamage(player.getComponent<PlayerInventory>().WeaponDamage("left", enemPos));
			}
		}
		if (player.getComponent<PlayerInventory>().HasRight())
		{
			if (sqrt(pow((enemPos.x - (playerPos.x + 64)), 2) + pow((enemPos.y - playerPos.y - 10), 2)) <=
				player.getComponent<PlayerInventory>().GetRange("right") * 10)
			{
				Mix_PlayChannel(2, soundEfect, 0);
				e->getComponent<Stats>().TakeDamage(player.getComponent<PlayerInventory>().WeaponDamage("right", enemPos));
			}
		}
		if (e->getComponent<Stats>().GetHealth() <= 0)
		{
			player.getComponent<Level>().GainExp(e->getComponent<Stats>().GetExp());
			if (rand() % 100 < 5)
				assets->CreateItem(Vector2D(enemPos.x, enemPos.y), "weapons");
			e->destroy();
		}
	}

	for (auto& i : items)
	{
		if (i->hasComponent<Item>())
		{
			if (i->getComponent<Item>().GetEquiped() == false)
			{
				if (Collision::AABB(playerCol, i->getComponent<ColliderComponent>().collider))
				{
					if (Game::event.type == SDL_KEYUP && Game::isRunning && !Game::startScreen)
					{

						switch (Game::event.key.keysym.sym) {
						case SDLK_q:
							std::cout <<"I pressed once\n";
							if (player.getComponent<PlayerInventory>().UpgradeLeft(*i) == false)
								player.getComponent<PlayerInventory>().EquipLeft(*i);
							break;
						case SDLK_e:
							if (player.getComponent<PlayerInventory>().UpgradeRight(*i) == false)
								player.getComponent<PlayerInventory>().EquipRight(*i);
							break;
						}
					}
				}
			}
		}
	}

	if (!Game::startScreen && !Game::hasRoundStarted)
	{
		ss.str("");
		ss << player.getComponent<Stats>().GetMaxHealth();
		statusBtn1.getComponent<UILabel>().SetLabelText(ss.str(), "default");
		ss.str("");
		ss << player.getComponent<Stats>().GetRegen();
		statusBtn2.getComponent<UILabel>().SetLabelText(ss.str(), "default");
		ss.str("");
		ss << player.getComponent<Stats>().GetDamage();
		statusBtn3.getComponent<UILabel>().SetLabelText(ss.str(), "default");
		ss.str("");
		ss << player.getComponent<Stats>().GetArmor();
		statusBtn4.getComponent<UILabel>().SetLabelText(ss.str(), "default");
		ss.str("");
		ss << player.getComponent<Stats>().GetAttackSpeed();
		statusBtn5.getComponent<UILabel>().SetLabelText(ss.str(), "default");
		ss.str("");
		ss << player.getComponent<Stats>().GetSpeed();
		statusBtn6.getComponent<UILabel>().SetLabelText(ss.str(), "default");
		ss.str("");
		ss << "Points: " << player.getComponent<Level>().GetPoints();
		points.getComponent<UILabel>().SetLabelText(ss.str(), "default");

		if (Game::event.type == SDL_MOUSEBUTTONDOWN)
		{
			int xMouse, yMouse;
			if (SDL_BUTTON(SDL_GetMouseState(&xMouse, &yMouse)) == SDL_BUTTON_LEFT)
			{
				if (player.getComponent<Level>().GetPoints() > 0)
				{
					if (statusBtn1.getComponent<Button>().isHitting(xMouse, yMouse))
						player.getComponent<Level>().UsePoints(0);
					else if (statusBtn2.getComponent<Button>().isHitting(xMouse, yMouse))
						player.getComponent<Level>().UsePoints(1);
					else if (statusBtn3.getComponent<Button>().isHitting(xMouse, yMouse))
						player.getComponent<Level>().UsePoints(2);
					else if (statusBtn4.getComponent<Button>().isHitting(xMouse, yMouse))
						player.getComponent<Level>().UsePoints(3);
					else if (statusBtn5.getComponent<Button>().isHitting(xMouse, yMouse))
						player.getComponent<Level>().UsePoints(4);
					else if (statusBtn6.getComponent<Button>().isHitting(xMouse, yMouse))
						player.getComponent<Level>().UsePoints(5);

					if (player.getComponent<Level>().GetPoints() == 0)
					{
						statusBtn1.getComponent<Button>().changeColor(GetColor("grey"));
						statusBtn2.getComponent<Button>().changeColor(GetColor("grey"));
						statusBtn3.getComponent<Button>().changeColor(GetColor("grey"));
						statusBtn4.getComponent<Button>().changeColor(GetColor("grey"));
						statusBtn5.getComponent<Button>().changeColor(GetColor("grey"));
						statusBtn6.getComponent<Button>().changeColor(GetColor("grey"));
					}
				}
				if (roundscreen.getComponent<Button>().isHitting(xMouse, yMouse))
				{
					Game::startRoundTime = std::chrono::system_clock::now();
					Game::hasRoundStarted = true;
					money.getComponent<UILabel>().SetPosText(12, 92);
				}
			}
		}
	}

	if (hasRoundStarted == true)
	{
		switch (currentRound) {
		case 1:
			if (((roundTime + int(elapsed_seconds.count())) % 4 == 0) && !enemySpawned)
			{
				for(int it=0; it < spawnRate; ++it)
					assets->CreateWave(1 + rand() % 4, 1, 1);
				enemySpawned = true;
			}else if ((roundTime + int(elapsed_seconds.count()) - 1) % 4 == 0)
				enemySpawned = false;
			break;
		case 2:
			if (((roundTime + int(elapsed_seconds.count())) % 3 == 0) && !enemySpawned)
			{
				for (int it = 0; it < spawnRate; ++it)
				{
					assets->CreateWave(2 + rand() % 6, 1, 1);
					assets->CreateWave(1 + rand() % 2, 2, 1);
				}
				enemySpawned = true;
			}
			else if ((roundTime + int(elapsed_seconds.count()) - 1) % 3 == 0)
				enemySpawned = false;
			break;
		case 3:
			if (((roundTime + int(elapsed_seconds.count())) % 3 == 0) && !enemySpawned)
			{
				for (int it = 0; it < spawnRate; ++it)
				{
					assets->CreateWave(4 + rand() % 8, 1, 2);
					assets->CreateWave(2 + rand() % 4, 2, 1);
					assets->CreateWave(0 + rand() % 2, 3, 1);
				}
				enemySpawned = true;
			}
			else if ((roundTime + int(elapsed_seconds.count()) - 1) % 3 == 0)
				enemySpawned = false;
			break;
		case 4:
			if (((roundTime + int(elapsed_seconds.count())) % 2 == 0) && !enemySpawned)
			{
				assets->CreateWave(5 + rand() % 98, 1, 2);
				assets->CreateWave(2 + rand() % 4, 2, 2);
				assets->CreateWave(0 + rand() % 3, 3, 1);
				enemySpawned = true;
			}
			else if ((roundTime + int(elapsed_seconds.count()) - 1) % 2 == 0)
				enemySpawned = false;
			break;
		case 5:
			if (((roundTime + int(elapsed_seconds.count())) % 3 == 0) && !enemySpawned)
			{
				assets->CreateWave(6 + rand() % 10, 1, 3);
				assets->CreateWave(3 + rand() % 6, 2, 2);
				assets->CreateWave(1 + rand() % 4, 3, 1);
				enemySpawned = true;
			}
			else if ((roundTime + int(elapsed_seconds.count()) - 1) % 3 == 0)
				enemySpawned = false;
			break;
		}

		//ROUND OVER
		if (roundTime + int(elapsed_seconds.count()) == 0)
		{
			currentRound++;
			if (currentRound < 6)
			{
				money.getComponent<UILabel>().SetPosText(50, windowSize.y * 3 / 4 - 50);
				Game::hasRoundStarted = false;
				for (auto& e : enemies)
					e->destroy();
				for (auto& p : projectiles)
					p->destroy();
				for (auto& i : items)
				{
					if (i->getComponent<Item>().GetEquiped() == false)
						i->destroy();
				}
				std::string text = "Round " + std::to_string(currentRound);
				roundCounter.getComponent<UILabel>().SetLabelText(text, "default");
			}
			else
			{
				std::cout << "\n\nGame ended.You won!\n ----Thank you for playing my demo----\n\n";
				Game::isRunning = false;
			}
		}
	}

	camera.x = playerPos.x - windowSize.x / 2 + 32;
	camera.y = playerPos.y - windowSize.y / 2 + 32;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > 480)
		camera.x = 480;
	if (camera.y > 678)
		camera.y = 678;

	if (playerHealth <= 0)
	{
		Game::isRunning = false;
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}
	for (auto& p : projectiles)
	{
		p->draw();
	}
	for (auto& i : items)
	{
		i->draw();
	}

	if (hasRoundStarted)
	{
		health.draw();
		expbar.draw();
		timeCounter.draw();
	}

	if (!hasRoundStarted)
	{
		roundscreen.draw();
		statusBtn1.draw();
		statusBtn2.draw();
		statusBtn3.draw();
		statusBtn4.draw();
		statusBtn5.draw();
		statusBtn6.draw();
		points.draw();
	}

	money.draw();
	roundCounter.draw();

	if (startScreen == true)
	{
		loadscreen.draw();
		button1.draw();
		button2.draw();
	}
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	Mix_FreeChunk(soundEfect);
	Mix_FreeMusic(bgMusic);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	bgMusic = nullptr;
	soundEfect = nullptr;

	Mix_Quit();
	SDL_Quit();
	std::cout << "Memory was cleaned!...\n";
}

bool Game::checkIsRunning()
{
	return isRunning;
}