#pragma once
#include "Sorcerer.h"
struct CombatContext { 
	void Taunt(Sorcerer* user, Character* target);
	void WorldCuttingSlashReady(Sorcerer* user);
	int GetRandomNumber(int min, int max);
};