#include "Sorcerer.h"
#include "InvertedSpearOfHeaven.h"

import std;

void InvertedSpearofHeaven::UseTool(Sorcerer* user, Character* target) {
	target->DamageBypass(special_tool_damage * special_tool_mult);
	std::println("{} attacks {} with the Inverted Spear of Heaven!", user->GetName(), target->GetName());
}
std::string InvertedSpearofHeaven::GetName() const {
	return "The Inverted Spear of Heaven";
}