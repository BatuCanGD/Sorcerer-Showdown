#include <string>
#include <vector>
#include <memory>

#include "Character.h"
#include "Sorcerer.h"
#include "Technique.h"


Character::Character(double hp, double ce, double regen)
	: health(hp),
	max_health(hp),
	previous_health(hp),
	cursed_energy(ce),
	max_cursed_energy(ce),
	ce_regen_efficiency(regen),
	is_heavenly_restricted(cursed_energy < 0.0) { // -1 or -0.01 or whatever you prefer to make a character heavenly restricted
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

void Character::Damage(double h) {
	if (CanBeHit() && !is_invulnerable) {
		health = std::max(health - h, 0.0);
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
double Character::GetMaxCharCE() const {
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

bool Character::IsAttacking() const {
	return current_attack == AttackType::Basic;
}
bool Character::IsUsingTechnique() const {
	return current_attack == AttackType::Technique;
}
bool Character::IsUsingDomain() const {
	return current_attack == AttackType::Domain;
}
bool Character::IsBlocking() const {
	return block_state == BlockState::Blocking;
}

void Character::SetAttack() {
	current_attack = AttackType::Basic;
}
void Character::SetUsingTechnique() {
	current_attack = AttackType::Technique;
}
void Character::SetUsingDomain() {
	current_attack = AttackType::Domain;
}
void Character::SetBlocking() {
	block_state = BlockState::Blocking;
}

void Character::ResetAttackState() {
	current_attack = AttackType::None;
}
void Character::ResetBlockState() {
	block_state = BlockState::None;
}


bool Character::IsCharacterStunned() const {
	return is_stunned;
}
bool Character::IsHeavenlyRestricted() const {
	return is_heavenly_restricted;
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

std::string Character::GetName()const {
	return "Character";
}