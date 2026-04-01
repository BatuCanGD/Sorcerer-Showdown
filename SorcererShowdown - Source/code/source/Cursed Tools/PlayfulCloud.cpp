#include "PlayfulCloud.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

void PlayfulCloud::UseTool(Sorcerer* user, Character* target) {
	Sorcerer* s = static_cast<Sorcerer*>(target);
	double dmg = GetCalculatedStrength(user);
	target->Damage(dmg);
	std::println("{} {}attacks{} {} with {}", user->GetNameWithID(), Color::Red, Color::Clear, s->GetNameWithID(), this->GetName());
}
double PlayfulCloud::GetCalculatedStrength(Sorcerer* user)  const {
	if (!user->IsHeavenlyRestricted()) {
		return base_tool_damage + ((user->GetCharacterCE() / 300.0) * 1.25);
	}
	else {
		return base_tool_damage + (user->GetCharacterMaxHealth() / 10.0);
	}
}
std::string PlayfulCloud::GetName() const {
	return "\033[31mPlayful Cloud\033[0m";
}
std::string PlayfulCloud::GetSimpleName() const {
	return "Playful Cloud";
}