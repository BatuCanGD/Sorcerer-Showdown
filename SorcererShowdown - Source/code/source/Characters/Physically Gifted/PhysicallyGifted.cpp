#include "PhysicallyGifted.h"

PhysicallyGifted::PhysicallyGifted(double hp, double str) 
	: Character(hp, -1, -1) 
{
	is_heavenly_restricted = true;
	strength = str;

	max_ce_reinforcement = 0.0;
	current_ce_reinforcement = 0.0;

	global_id_counter++;
	unique_id = global_id_counter;
}

std::unique_ptr<Character> PhysicallyGifted::Clone() const {
	return nullptr;
}

bool PhysicallyGifted::IsPhysicallyGifted() const {
	return true;
}

double PhysicallyGifted::GetStrength() const {
	return strength;
}
double PhysicallyGifted::GetStrengthDamage()const {
	return 1.0 + (strength / strength_to_damage_ratio);
}

double PhysicallyGifted::GetDamageReinforcement() const {
	return 1.0 + (strength / strength_to_reinforcement_ratio);
}

std::string PhysicallyGifted::GetSimpleName() const {
	return "Physically Gifted";
}
std::string PhysicallyGifted::GetName() const {
	return "Heavenly Restricted User";
}
bool PhysicallyGifted::CanBeHit() const {
	return true;
}