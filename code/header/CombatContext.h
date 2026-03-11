#pragma once
#include "Sorcerer.h"
class CombatContext {
public:
	virtual ~CombatContext() = default;
	void Taunt(Sorcerer* user, Character* target);
	int GetRandomNumber(int min, int max);
};

class WorldCuttingSlash : public CombatContext {
public:	
	void WorldCuttingSlashReady(Sorcerer* user);
};