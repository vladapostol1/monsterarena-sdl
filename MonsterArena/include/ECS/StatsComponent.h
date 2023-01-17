#pragma once
#include <math.h>
#include <chrono>

class Stats : public Component
{
private:
	int healthAmount = 5;
	int maxHealth = 5;
	int healthRegen;
	int attakSpeed;
	int damage;
	int armor;
	int speed;
	int exp;
	bool type;

	std::chrono::time_point<std::chrono::system_clock> lastRegen;
	std::chrono::duration<double> timeToRegen;
public:
	Stats(int ammount = 5, int regen = 0, int dmg = 1, int armr = 0, int atSpd = 5, int movSpd = 1, int xp = 0, bool tp = false)
	{
		maxHealth = ammount;
		healthAmount = ammount;
		healthRegen = regen;
		damage = dmg;
		armor = armr;
		attakSpeed = atSpd;
		speed = movSpd;
		type = tp;
		exp = xp;
	}
	~Stats()
	{}

	void init()
	{
		lastRegen = std::chrono::system_clock::now();
		healthAmount = maxHealth;
		timeToRegen = std::chrono::milliseconds{ 5000 };
	}

	void update()
	{
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		if (healthAmount < maxHealth && end - lastRegen > timeToRegen)
		{
			if ((healthAmount + healthRegen) > maxHealth)
				healthAmount = maxHealth;
			else
				healthAmount += healthRegen;
			lastRegen = std::chrono::system_clock::now();
		}
	}

	int GetHealth()
	{
		return healthAmount;
	}

	int GetMaxHealth()
	{
		return maxHealth;
	}

	int GetRegen()
	{
		return healthRegen;
	}

	int GetSpeed()
	{
		return speed;
	}

	int GetDamage()
	{
		return damage;
	}

	int GetAttackSpeed()
	{
		return attakSpeed;
	}

	int GetArmor()
	{
		return armor;
	}

	int GetExp()
	{
		return exp;
	}

	bool GetType()
	{
		return type;
	}

	void TakeDamage(int dmg)
	{
		healthAmount -= dmg * (100 - int(sqrt(armor) * 2)) / 100;
	}

	void IncreaseStat(int id, int points)
	{
		switch (id) {
		case 0:
			maxHealth += points;
			break;
		case 1:
			healthRegen += points;
			break;
		case 2:
			damage += points;
			break;
		case 3:
			armor += points;
			break;
		case 4:
			attakSpeed += points;
			break;
		case 5:
			speed += points;
			break;
		default:
			std::cout << "The id does not match any stat!!!\n";
			break;
		}
	}
};