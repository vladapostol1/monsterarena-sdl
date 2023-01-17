#pragma once

class Enemie : public Component
{
private :
	TransformComponent* transform;
	SpriteComponent* sprite;
	Stats* stats;
	Vector2D target;
	bool ranged;
	std::chrono::time_point<std::chrono::system_clock> lastShoot;
	std::chrono::duration<double> timeToShoot;

	std::chrono::time_point<std::chrono::system_clock> lastAttack;
	std::chrono::duration<double> timeToAttack;

public:
	void init() override
	{
		lastShoot = std::chrono::system_clock::now();
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		stats = &entity->getComponent<Stats>();
		transform->speed = stats->GetSpeed();
		ranged = stats->GetType();
		lastAttack = std::chrono::system_clock::now();
		timeToShoot = std::chrono::milliseconds{ 3000 };
		timeToAttack = std::chrono::milliseconds{ 1500 } - std::chrono::milliseconds{ 10 } * stats->GetAttackSpeed();
	}

	void update() override
	{
		if (Game::hasRoundStarted)
		{
			target = Game::playerPos;
			std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
			if (transform->position.x < target.x)
			{
				sprite->spriteFlip = SDL_FLIP_NONE;
				transform->velocity.x = 1;
			}
			else if (transform->position.x > target.x)
			{
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				transform->velocity.x = -1;
			}
			else transform->velocity.x = 0;
			if (transform->position.y < target.y)
				transform->velocity.y = 1;
			else
				if (transform->position.y > target.y)
					transform->velocity.y = -1;
				else transform->velocity.y = 0;
			if (ranged)
			{
				if (end - lastShoot > timeToShoot)
				{
					Shoot();
					lastShoot = end;
				}
			}
		}
	}

	int DoDamage()
	{
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		if (end - lastAttack > timeToAttack)
		{
			lastAttack = std::chrono::system_clock::now();
			return stats->GetDamage();
		}
		return 0;
	}

	void KnockBack()
	{
		transform->position.x -= 20 * transform->velocity.x;
		transform->position.y -= 20 * transform->velocity.y;
	}

	void Shoot()
	{
		Game::assets->CreateProjectile(Vector2D(transform->position.x + 2 * transform->velocity.x, transform->position.y + 2 * transform->velocity.y), 500, 5, 1, Vector2D(transform->velocity.x, transform->velocity.y), "projectile");
	}
};
