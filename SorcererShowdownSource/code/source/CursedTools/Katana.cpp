#include "code/header/CursedTools/Katana.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



Katana::Katana() {
	name = "a Katana";
	color = "\033[36m";
}
void Katana::UseTool(Character* user, Character* target) {
	double total_damage = 20.0;
	if (user->IsPhysicallyGifted()) {
		auto pg = static_cast<PhysicallyGifted*>(user);
		total_damage = base_damage * pg->GetStrengthDamage();
	}
	else if (user->IsaCurseUser()) {
		auto sr = static_cast<CurseUser*>(user);
		total_damage = base_damage * (1 + (sr->GetCharacterCE() / 5000));
	}

	target->Damage(total_damage);
	std::println("{} attacks {} using {}", user->GetNameWithID(), target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> Katana::Clone() const {
	return std::make_unique<Katana>(*this);
}