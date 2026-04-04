#include "PhysicallyGifted.h"
#include "Limitless.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

PhysicallyGifted::PhysicallyGifted(double hp, double str) 
	: Character(hp, -1, -1) 
{
	is_heavenly_restricted = true;
	strength = str;

	max_ce_reinforcement = 0.0;
	current_ce_reinforcement = 0.0;
}

std::unique_ptr<Character> PhysicallyGifted::Clone() const {
	return nullptr;
}

void  PhysicallyGifted::Attack(Character* target) {
    if (auto* target_sorcerer = dynamic_cast<Sorcerer*>(target)) {
        if (auto* limitless = dynamic_cast<Limitless*>(target_sorcerer->GetTechnique())) {
            bool has_spear = cursed_tool && cursed_tool->GetSimpleName() == "The Inverted Spear of Heaven";
            if (limitless->CheckInfinity() && !has_spear) {
                std::println("{}'s attack was blocked by {}'s {}Infinity{}!",
                    this->GetNameWithID(), target_sorcerer->GetNameWithID(), Color::Cyan, Color::Clear);
                return;
            }
        }
    }

    if (cursed_tool) {
        cursed_tool->UseTool(this, target);
    }
    else {
        std::println("{} strikes {} with their bare hands!", this->GetNameWithID(), target->GetNameWithID());
        target->Damage(10.0 * this->GetStrengthDamage());
    }
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