#include "Katana.h"
#include "PhysicallyGifted.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

void Katana::UseTool(Character* user, Character* target) {
	auto pg = dynamic_cast<PhysicallyGifted*>(user);
	double total_damage = pg ?
		(base_tool_damage * special_tool_mult) * pg->GetStrengthDamage() :
		(base_tool_damage * special_tool_mult) * (1 + (user->GetCharacterCE() / 5000));

	target->Damage(total_damage);
	std::println("{} attacks {} using {}", user->GetNameWithID(), target->GetNameWithID(), this->GetName());
}
std::string Katana::GetName() const {
	return "\033[36ma Katana\033[0m";
}
std::string Katana::GetSimpleName() const {
	return "a Katana";
}