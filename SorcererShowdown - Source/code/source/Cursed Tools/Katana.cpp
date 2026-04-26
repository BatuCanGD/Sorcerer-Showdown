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
	auto sr = dynamic_cast<CurseUser*>(user);
	double total_damage;
	if (pg) {
		total_damage = (base_tool_damage * special_tool_mult) * pg->GetStrengthDamage();
	}
	else if (sr) {
		total_damage = (base_tool_damage * special_tool_mult) * (1 + (sr->GetCharacterCE() / 5000));
	}
	else {
		total_damage = 10.0;
	}

	target->Damage(total_damage);
	std::println("{} attacks {} using {}", user->GetNameWithID(), target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> Katana::Clone() const {
	return std::make_unique<Katana>(*this);
}