#include "TransfiguredHuman.h"
#include "BattlefieldHeader.h"
#include "Utils.h"

import std;

TransfiguredHuman::TransfiguredHuman() : CursedSpirit(50.0, 10.0, 0.0) {
	passive_health_regen = 0.0;
	current_ce_reinforcement = 0.0;
	max_ce_reinforcement = 0.0;
	black_flash_chance = 0;
};

std::unique_ptr<Character> TransfiguredHuman::Clone() const {
	return nullptr;
}
std::string TransfiguredHuman::GetSimpleName() const {
	return "Transfigured Human";
}
std::string TransfiguredHuman::GetName() const {
	return "Transfigured Human";
}
bool TransfiguredHuman::CanBeHit() const {
	return true;
}
void TransfiguredHuman::OnCharacterTurn(Character*, Battlefield& bf) {
	Character* target = nullptr;
	for (const auto& tar : bf.battlefield) {
		if (tar.get() == this ||
			tar.get()->GetSimpleName() == "Transfigured Human" ||
			tar.get()->GetSimpleName() == "Mahito") continue;
		if (target == nullptr) target = tar.get();

		if (GetRandomNumber(0, 1) == 1 || !target) {
			target = tar.get();
		}
	}
	this->Attack(target);
}