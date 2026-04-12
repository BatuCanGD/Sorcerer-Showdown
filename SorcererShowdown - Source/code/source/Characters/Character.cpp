#include "Character.h"
#include "PhysicallyGifted.h"
#include "Utils.h"
#include "CursedTool.h"
#include "Sorcerer.h"
#include "Techniques.h"

import std;

int Character::global_id_counter = 0;

Character::Character(double hp, double ce, double regen)
	: health(hp),
	max_health(hp),
	previous_health(hp),
	cursed_energy(ce),
	max_cursed_energy(ce),
	ce_regen_efficiency(regen){
}
Character::~Character() = default;

void Character::OnCharacterTurn(Character*, std::vector<std::unique_ptr<Character>>&) {
	std::println("Base Character Initialized Text");
}

bool Character::CanBeAssignedID() const {
	return true;
}

void Character::Attack(Character* target) {
	std::println("Placeholder character class virtual attack function, {}", target->GetName());
}

void Character::AssignID() {
	if (CanBeAssignedID()) {
		global_id_counter++;
		unique_id = global_id_counter;
	}
}

bool Character::IsThePlayer() const {
	return is_player;
}

void Character::SetAsPlayer(bool p) {
	is_player = p;
}

void Character::SetVulnerability(bool t) {
	is_invulnerable = t;
}

void Character::SetHealth(double h) {
	health = h;
}

void Character::SetCursedEnergy(double c) {
	cursed_energy = c;
}

void Character::SetMaxCursedEnergy(double c) {
	max_cursed_energy = c;
}

void Character::SetCursedEnergyRegen(double c) {
	ce_regen_efficiency = c;
}

void Character::Damage(double h) {
	if (CanBeHit() && !is_invulnerable) {
		health = std::max(health - (h / this->GetDamageReinforcement()), 0.0);
	}
}
void Character::DamageBypass(double h) {
	if (is_invulnerable) return;
	health = std::max(health - h, 0.0);
}

void Character::Regen(double h) {
	health = std::min(health + h, max_health);
}

double Character::GetCharacterHealth() const {
    return health;
}
double Character::GetCharacterMaxHealth() const {
	return max_health;
}
double Character::GetCharacterCE() const {
	return cursed_energy;
}

void Character::SpendCE(double c) { 
	cursed_energy = std::max(cursed_energy - c, 0.0);
}

void Character::RegenCE() {
	if (is_heavenly_restricted) return;
	cursed_energy = std::min(cursed_energy + ce_regen_efficiency, max_cursed_energy);
}

double Character::GetCEregen() const {
	return ce_regen_efficiency;
}

double Character::GetCharacterMaxCE() const {
	return max_cursed_energy;
}
double Character::GetCharacterPreviousHealth() const {
	return previous_health;
}

void Character::SetStunState(bool s) {
	is_stunned = s;
}

void Character::UpdatePreviousHP() {
	previous_health = health;
}

bool Character::IsCharacterStunned() const {
	return is_stunned;
}

bool Character::CEMoreThanMax(double c) const {
	return this->GetCharacterCE() > this->GetCharacterMaxCE() * c;
}
bool Character::HPMoreThanMax(double h) const {
	return this->GetCharacterHealth() > this->GetCharacterMaxHealth() * h;
}

void Character::ClearStunTime() {
	if (stun_duration <= 0) {
		is_stunned = false;
		stun_duration = 2;
	}
	else {
		stun_duration--;
	}
}

int Character::GetID() const {
	return unique_id;
}

std::string Character::GetNameWithID()const {
	return std::format("{} ({})", this->GetName(), unique_id);
}

bool Character::IsaSorcerer() const {
	return false;
}
bool Character::IsShikigami() const {
	return false;
}
bool Character::IsPhysicallyGifted() const {
	return false;
}
bool Character::IsaCurseUser() const {
	return false;
}

double Character::GetReinforcement() const {
	return current_ce_reinforcement;
}
double Character::GetMaxReinforcement()const {
	return max_ce_reinforcement;
}
double Character::GetDamageReinforcement()const {
	if (max_ce_reinforcement <= 0.0) return 1.0;
	return 1.0 + ((current_ce_reinforcement / max_ce_reinforcement) * 2);
}

void Character::CursedToolChoice(int choice) {
	if (choice == 0) {
		if (cursed_tool != nullptr) {
			std::println("{}{} put {} away.{}", Color::BrightRed, this->GetNameWithID(), cursed_tool->GetName(), Color::Clear);
			inventory_curse.push_back(std::move(cursed_tool));
			cursed_tool = nullptr;
		}
		return;
	}

	size_t inv_index = choice - 1;
	if (inv_index >= 0 && inv_index < inventory_curse.size()) {
		if (cursed_tool != nullptr) {
			inventory_curse.push_back(std::move(cursed_tool));
		}
		cursed_tool = std::move(inventory_curse[inv_index]);
		inventory_curse.erase(inventory_curse.begin() + inv_index);

		std::println("{}{} equipped {}!{}", Color::Cyan, this->GetNameWithID(), cursed_tool->GetName(), Color::Clear);
	}
	else {
		std::println("{}Invalid tool choice.{}", Color::Red, Color::Clear);
	}
}

void Character::EquipToolByName(const std::string& weaponName) {
	for (int i = 0; i < inventory_curse.size(); ++i) {
		if (inventory_curse[i]->GetSimpleName() == weaponName) {
			CursedToolChoice(i + 1);
			return;
		}
	}
}

void Character::SetCurrentReinforcement(double r) {
	current_ce_reinforcement = std::clamp(r, 0.0, max_ce_reinforcement);
}
void Character::SetMaxReinforcement(double max) {
	max_ce_reinforcement = max;
}
void Character::AddReinforcement(double r) {
	current_ce_reinforcement = std::clamp(current_ce_reinforcement + r, 0.0, max_ce_reinforcement);
}

void Character::TickCharacterSpecialty() {};

void Character::TickReinforcement() {
	if (this->IsPhysicallyGifted()) return;
	if (current_ce_reinforcement <= 0.0) return;
	double maintain_cost = current_ce_reinforcement;
	this->SpendCE(maintain_cost);
	if (this->GetCharacterCE() < this->GetReinforcement()) {
		current_ce_reinforcement = 0.0;
		std::println("{}'s CE reinforcement collapsed due to a lack of Cursed Energy!", this->GetName());
	}
}

const std::vector<std::unique_ptr<CursedTool>>& Character::GetCursedTools() const {
	return inventory_curse;
}

CursedTool* Character::GetTool() const {
	return cursed_tool.get();
}

void Character::Taunt(Character* taunted) const { // pure aura
	if (!taunted) return;
	const double healthy_threshold = 0.70;
	const double injured_threshold = 0.40;
	const double critical_threshold = 0.20;

	int taunt_type = GetRandomNumber(1, 4);

	if (this->HPMoreThanMax(healthy_threshold)) {
		switch (taunt_type) {
		case 1:
			std::println("I'm surprised you've even managed to scratch me this much {}!", taunted->GetNameWithID());
			break;
		case 2:
			std::println("Is that all you've got, {}? I expected more from you!", taunted->GetNameWithID());
			break;
		case 3:
			std::println("You're not even worth my time, {}!", taunted->GetNameWithID());
			break;
		default:
			std::println("You should just give up now, {}!", taunted->GetNameWithID());
		}
	}
	else if (this->HPMoreThanMax(injured_threshold)) {
		switch (taunt_type) {
		case 1:
			std::println("You're starting to annoy me, {}. Keep it up and I'll make you regret it!", taunted->GetNameWithID());
			break;
		case 2:
			std::println("You're not doing too bad, {}. But don't get too confident just yet!", taunted->GetNameWithID());
			break;
		case 3:
			std::println("Huh, you're actually putting up a fight, {}. I might have to take you more seriously!", taunted->GetNameWithID());
			break;
		default:
			std::println("You're not bad, {}. But I'm still better!", taunted->GetNameWithID());

		}
	}
	else if (this->HPMoreThanMax(critical_threshold)) {
		switch (taunt_type) {
		case 1:
			std::println("You're really starting to piss me off, {}. I'll make you regret your actions!", taunted->GetNameWithID());
			break;
		case 2:
			std::println("You're actually pretty strong, but it won't be enough to defeat me, {}!", taunted->GetNameWithID());
			break;
		case 3:
			std::println("You're really starting to get on my nerves, {}. I might have to end this quickly!", taunted->GetNameWithID());
			break;
		default:
			std::println("You think a few hits will stop me, {}? Im just getting warmed up!", taunted->GetNameWithID());
		}
	}
	else {
		switch (taunt_type) {
		case 1:
			std::println("You think this is over, {}? I'll drag you to the grave with me!", taunted->GetNameWithID());
			break;
		case 2:
			std::println("Blood for blood, {}! You won't leave this place alive!", taunted->GetNameWithID());
			break;
		case 3:
			std::println("You're really starting to piss me off, {}. I might have to end this quickly!", taunted->GetNameWithID());
			break;
		default:
			std::println("I'll make you wish you were never born {}!", taunted->GetNameWithID());
		}
	}
}