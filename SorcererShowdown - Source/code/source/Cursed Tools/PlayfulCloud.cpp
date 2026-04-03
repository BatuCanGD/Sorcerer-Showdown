#include "PlayfulCloud.h"
#include "Sorcerer.h"
#include "PhysicallyGifted.h"
#include "Utils.h"

import std;

void PlayfulCloud::UseTool(Character* user, Character* target) {
	double dmg = GetCalculatedStrength(user);
	target->Damage(dmg);
	std::println("{} {}attacks{} {} with {}", user->GetNameWithID(), Color::Red, Color::Clear, target->GetNameWithID(), this->GetName());
}
std::string PlayfulCloud::GetName() const {
	return "\033[31mPlayful Cloud\033[0m";
}
std::string PlayfulCloud::GetSimpleName() const {
	return "Playful Cloud";
}