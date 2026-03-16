#pragma once
#include "Sorcerer.h"
class CombatContext {
protected:
	int GetRandomNumber(int min, int max);
public:
	virtual ~CombatContext() = default;
	void Taunt(Sorcerer* user, Character* target);
};

class WorldCuttingSlash : public CombatContext {
public:	
	void WorldCuttingSlashReady(Sorcerer* user);
};