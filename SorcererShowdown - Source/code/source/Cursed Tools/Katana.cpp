#include "Katana.h"
#include "Sorcerer.h"

import std;

void Katana::UseTool(Sorcerer* user, Character* target) {
	target->Damage((base_tool_damage * special_tool_mult) * (1 + (user->GetCharacterCE() / 5000)));
	std::println("{} attacks {} using their katana!", user->GetName(), target->GetName());
}
std::string Katana::GetName() const {
	return "a Katana";
}