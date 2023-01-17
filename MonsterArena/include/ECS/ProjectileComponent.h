#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp, int dmg, Vector2D vel) : range(rng) , speed(sp), damage(dmg), velocity(vel)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override
	{
		distance += speed;

		if (distance > range)
		{
			entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w ||
			transform->position.x < Game::camera.x ||
			transform->position.y > Game::camera.y + Game::camera.h ||
			transform->position.y < Game::camera.y)
		{
			entity->destroy();
		}
	}

	int GetDamage()
	{
		return damage;
	}


private:

	TransformComponent* transform;
	Vector2D velocity;

	int range = 0;
	int speed = 0;
	int distance = 0;
	int damage = 0;
};