#include "Sorcerer.h"
#include "InvertedSpearOfHeaven.h"
#include "Utils.h"

import std;

void InvertedSpearofHeaven::UseTool(Sorcerer* user, Character* target) {
	Sorcerer* s = static_cast<Sorcerer*>(target);
	target->DamageBypass(special_tool_damage * special_tool_mult);
	std::println("{} {}attacks{} {} with {}", user->GetNameWithID(),Color::Red,Color::Clear, s->GetNameWithID(), this->GetName());
}
std::string InvertedSpearofHeaven::GetName() const {
	return "\033[33mThe Inverted Spear of Heaven\033[0m";
}
std::string InvertedSpearofHeaven::GetSimpleName()const {
	return "The Inverted Spear of Heaven";
}