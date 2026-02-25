#include "Character.h"


Character::Character(double hp, double ce)
    : health(hp), cursed_energy(ce)
{
    is_heavenly_restricted = (cursed_energy <= 0.0);
}

void Character::Damage(double h) {
    health -= h;
}

void Character::Regen(double h) {
    health += h;
}

double Character::GetCharacterHealth() const {
    return health;
}
double Character::GetCharacterCE() const {
	return cursed_energy;
}

void Character::SpendCE(double c) {
	cursed_energy -= c;
}
void Character::RegenCE(double c) {
	cursed_energy += c;
}

void Character::SetStunState(bool s) {
	is_stunned = s;
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