#pragma once

#include "ECS.h"


class Item : public Component
{
private:
	int id;
	int damage;
	int range;
	int speed;
	int level;
	int protect;
	bool isEquiped;
	SpriteComponent* sprite;
public:
	Item()
	{
		id = 1 + (rand() % 4);
		level = 1;
		isEquiped = false;
	}
	Item(int ID, int lvl=1)
	{
		id = ID;
		level = lvl;
		isEquiped = false;
	}
	~Item()
	{}

	friend bool operator==(const Item& lhs, const Item& rhs)
	{
		if ((lhs.id == rhs.id) && (lhs.level == rhs.level) && (lhs.level < 3))
			return true;
		return false;
	}

	void init() override
	{
		sprite = &entity->getComponent<SpriteComponent>();
		SetStats(id, level);
		isEquiped = false;
	}

	bool Upgrade()
	{
		if (level < 3)
		{
			level++;
			SetStats(id, level);
			return true;
		}
		return false;
	}

	TransformComponent* GetTransform()
	{
		return &entity->getComponent<TransformComponent>();
	}

	SpriteComponent* GetSprite()
	{
		return sprite;
	}

	void SetEquiped(bool is)
	{
		isEquiped = is;
	}

	bool GetEquiped()
	{
		return isEquiped;
	}

	int GetRange()
	{
		return range;
	}

	int GetDamage()
	{
		return damage;
	}

	int GetSpeed()
	{
		return speed;
	}

	void SetStats(int ID, int lvl)
	{
		switch (ID) {
		case 1:
			switch (lvl) {
			case 1:
				damage = 10;
				range = 10;
				speed = 5;
				protect = 15;
				sprite->SetSrc(0, 0);
				break;
			case 2:
				damage = 15;
				range = 8;
				speed = 6;
				protect = 15;
				sprite->SetSrc(32, 0);
				break;
			case 3:
				damage = 20;
				range = 13;
				speed = 10;
				protect = 25;
				sprite->SetSrc(64, 0);
				break;
			}
			break;
		case 2:
			switch (lvl) {
			case 1:
				damage = 5;
				range = 7;
				speed = 10;
				protect = 10;
				sprite->SetSrc(0, 32);
				break;
			case 2:
				damage = 8;
				range = 7;
				speed = 15;
				protect = 12;
				sprite->SetSrc(32, 32);
				break;
			case 3:
				damage = 10;
				range = 8;
				speed = 20;
				protect = 15;
				sprite->SetSrc(64, 32);
				break;
			}
			break;
		case 3:
			switch (lvl) {
			case 1:
				damage = 10;
				range = 12;
				speed = 4;
				protect = 10;
				sprite->SetSrc(0, 64);
				break;
			case 2:
				damage = 20;
				range = 14;
				speed = 5;
				protect = 10;
				sprite->SetSrc(32, 64);
				break;
			case 3:
				damage = 30;
				range = 15;
				speed = 3;
				protect = 15;
				sprite->SetSrc(64, 64);
				break;
			}
			break;
		case 4:
			switch (lvl) {
			case 1:
				damage = 1;
				range = 6;
				speed = 5;
				protect = 35;
				sprite->SetSrc(0, 96);
				break;
			case 2:
				damage = 2;
				range = 7;
				speed = 5;
				protect = 50;
				sprite->SetSrc(32, 96);
				break;
			case 3:
				damage = 4;
				range = 7;
				speed = 8;
				protect = 65;
				sprite->SetSrc(64, 96);
				break;
			}
			break;
		}
	}
};

class PlayerInventory : public Component
{
private:
	Item* leftHand;
	Item* rightHand;
	bool hasLeftHand;
	bool hasRightHand;
	TransformComponent* transformLeft;
	TransformComponent* transformRight;
	Stats* stats;

	std::chrono::time_point<std::chrono::system_clock> lastAttackLeft;
	std::chrono::duration<double> timeToAttackLeft;
	std::chrono::time_point<std::chrono::system_clock> lastAttackRight;
	std::chrono::duration<double> timeToAttackRight;
public:
	PlayerInventory()
	{
		leftHand = new Item();
		hasLeftHand = true;
		hasRightHand = false;
	}
	~PlayerInventory()
	{}

	void init()
	{
		stats = &entity->getComponent<Stats>();
		lastAttackLeft = std::chrono::system_clock::now();
		lastAttackRight = std::chrono::system_clock::now();
	}

	void update() override
	{
		if (hasLeftHand)
		{
			if (leftHand->GetSprite()->GetRot() == false)
			{
				transformLeft->position.x = Game::playerPos.x - 20;
				transformLeft->position.y = Game::playerPos.y + 10;
			}
			if (std::chrono::system_clock::now() - lastAttackLeft > std::chrono::milliseconds{ 250 })
			{
				transformLeft->velocity.x = 0;
				transformLeft->velocity.y = 0;
				leftHand->GetSprite()->ChangeRotation(false);
			}
		}
		if (hasRightHand)
		{
			if (rightHand->GetSprite()->GetRot() == false)
			{
				transformRight->position.x = Game::playerPos.x + 20;
				transformRight->position.y = Game::playerPos.y + 10;
			}
			if (std::chrono::system_clock::now() - lastAttackRight > std::chrono::milliseconds{ 250 })
			{
				transformRight->velocity.x = 0;
				transformRight->velocity.y = 0;
				rightHand->GetSprite()->ChangeRotation(false);
			}
		}
	}

	void EquipLeft(Entity& newItem)
	{
		if (hasLeftHand == true)
		{
			hasLeftHand = false;
			leftHand->SetEquiped(false);
		}
		leftHand = &newItem.getComponent<Item>();
		leftHand->SetEquiped(true);
		transformLeft = &newItem.getComponent<TransformComponent>();
		transformLeft->speed = 3;
		newItem.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
		hasLeftHand = true;
		timeToAttackRight = std::chrono::milliseconds{ 1500 } - std::chrono::milliseconds{ 10 } *stats->GetAttackSpeed() - std::chrono::milliseconds{ 10 } *leftHand->GetSpeed();
	}

	void EquipRight(Entity& newItem)
	{
		if (hasRightHand == true)
		{
			hasRightHand = false;
			rightHand->SetEquiped(false);
		}
		rightHand = &newItem.getComponent<Item>();
		rightHand->SetEquiped(true);
		transformRight = &newItem.getComponent<TransformComponent>();
		transformRight->speed = 2;
		newItem.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
		hasRightHand = true;
		timeToAttackRight = std::chrono::milliseconds{ 1500 } - std::chrono::milliseconds{ 10 } *stats->GetAttackSpeed() - std::chrono::milliseconds{ 10 } *leftHand->GetSpeed();
	}

	bool UpgradeLeft(Entity& newItem)
	{
		if (hasLeftHand)
			if (newItem.getComponent<Item>() == *leftHand)
			{
				if(leftHand->Upgrade());
					newItem.destroy();
				return true;
			}
		return false;
	}

	bool UpgradeRight(Entity& newItem)
	{
	if(hasRightHand)
		if (newItem.getComponent<Item>() == *rightHand)
		{
			if(rightHand->Upgrade());
				newItem.destroy();
			return true;
		}
	return false;
	}

	bool HasLeft()
	{
		if (hasLeftHand)
			return true;
		return false;
	}

	bool HasRight()
	{
		if (hasRightHand)
			return true;
		return false;
	}

	int GetRange(std::string id)
	{
		if (id == "left")
			return leftHand->GetRange();
		else
			return rightHand->GetRange();
	}

	int WeaponDamage(std::string id, Vector2D target)
	{
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		if (id == "left" && (end - lastAttackLeft > timeToAttackLeft))
		{
			lastAttackLeft = end;
			return Attack(leftHand, transformLeft, target);
		}
		else if (id == "right" && (end - lastAttackRight > timeToAttackRight))
		{
			lastAttackRight = end;
			return Attack(rightHand, transformRight, target);
		}
		return 0;
	}

	int Attack(Item* hand, TransformComponent* transform, Vector2D target)
	{
		int tempDmg;
		if (hand == leftHand)
			transform = transformLeft;
		else
			transform = transformRight;

		if (transform->position.x < target.x)
			transform->velocity.x = 1;
		else
			transform->velocity.x = -1;

		if (transform->position.y < target.y)
			transform->velocity.y = 1;
		else
			transform->velocity.y = -1;

		hand->GetSprite()->ChangeRotation(true, transform->velocity.x * 3);
		tempDmg = hand->GetDamage();
		tempDmg = int(tempDmg + tempDmg * stats->GetAttackSpeed() / 100);
		return tempDmg;
	}
};