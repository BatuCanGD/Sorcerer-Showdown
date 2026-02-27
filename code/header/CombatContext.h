#pragma once
#include "Sorcerer.h"
struct CombatContext { 
	void Attack(Character* user, Character* target);
	void UseTechnique(Sorcerer* user, Character* target);
	void Block(Character* user);
	void CheckBlock(Sorcerer* target, Character* attacker);
	void Taunt(Sorcerer* user, Character* target);
	void WorldCuttingSlashReady(Sorcerer* user);
	void ResetAttackState(Character* user);
	void ResetBlockState(Character* user);
};