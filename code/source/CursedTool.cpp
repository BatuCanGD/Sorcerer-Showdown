#include "CursedTool.h"
#include "Sorcerer.h"
#include "Character.h"

import std;

void InvertedSpearofHeaven::UseTool(Sorcerer* user, Character* target) {
	target->DamageBypass(special_tool_damage * special_tool_mult);
	std::println("{} attacks {} with the Inverted Spear of Heaven!", user->GetName(), target->GetName());
}
std::string InvertedSpearofHeaven::GetName() const {
	return "The Inverted Spear of Heaven";
}

void PlayfulCloud::UseTool(Sorcerer* user, Character* target) {
	double dmg = GetCalculatedStrength(user);
	target->Damage(dmg);
	std::println("{} attacks {} with Playful Cloud!", user->GetName(), target->GetName());
}
double PlayfulCloud::GetCalculatedStrength(Sorcerer* user) {
	if (!user->IsHeavenlyRestricted()) { 
		return base_tool_damage + ((user->GetCharacterCE() / 300.0) * 1.25);
	}
	else {
		return base_tool_damage + (user->GetCharacterMaxHealth() / 10.0);
	}
}
std::string PlayfulCloud::GetName() const {
	return "Playful Cloud";
}