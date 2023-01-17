#pragma once

#include "ECS/ECS.h"

class Level : public Component
{
private:
	int level;
	int exp;
	int expToLevelUP;
	int gold;
	int statPoints;
	Stats* stats;
public:
	Level()
	{}
	~Level()
	{}

	void init()
	{
		level = 1;
		exp = 0;
		expToLevelUP = 20;
		statPoints = 5;
		gold = 0;
		stats = &entity->getComponent<Stats>();
	}

	void GainExp(int gainedExp)
	{
		exp += gainedExp;
		gold += gainedExp;
		if (expToLevelUP <= exp)
		{
			exp -= expToLevelUP;
			LevelUp();
		}
	}

	void LevelUp()
	{
		level++;
		expToLevelUP = expToLevelUP * 2 + 3 * level;
		stats->IncreaseStat(0, 1);
		stats->IncreaseStat(2, 1);
		statPoints++;
	}

	void UsePoints(int statId)
	{
		stats->IncreaseStat(statId, 1);
		statPoints--;
	}

	int ReturnLevel()
	{
		return level;
	}

	int ReturnExp()
	{
		return exp;
	}

	int ExpNeeded()
	{
		return expToLevelUP;
	}

	int ReturnGold()
	{
		return gold;
	}

	int GetPoints()
	{
		return statPoints;
	}
};
