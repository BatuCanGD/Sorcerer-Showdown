#include "Katana.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

void Katana::UseTool(Sorcerer* user, Character* target) {
	target->Damage((base_tool_damage * special_tool_mult) * (1 + (user->GetCharacterCE() / 5000)));
	std::println("{} attacks {} using {}", user->GetName(), target->GetName(), this->GetName());
}
std::string Katana::GetName() const {
	return "\033[36ma Katana\033[0m";
}
std::string Katana::GetSimpleName() const {
	return "a Katana";
}