#include "Katana.h"
#include "PhysicallyGifted.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

Katana::Katana() {
	tool_name = "a Katana";
	tool_color = "\033[36";
}
void Katana::UseTool(Character* user, Character* target) {
	auto pg = dynamic_cast<PhysicallyGifted*>(user);
	double total_damage = pg ?
		(base_tool_damage * special_tool_mult) * pg->GetStrengthDamage() :
		(base_tool_damage * special_tool_mult) * (1 + (user->GetCharacterCE() / 5000));

	target->Damage(total_damage);
	std::println("{} attacks {} using {}", user->GetNameWithID(), target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> Katana::Clone() const {
	return std::make_unique<Katana>(*this);
}