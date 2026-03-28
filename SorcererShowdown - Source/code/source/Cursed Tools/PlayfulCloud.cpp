#include "PlayfulCloud.h"
#include "Sorcerer.h"

import std;

void PlayfulCloud::UseTool(Sorcerer* user, Character* target) {
	double dmg = GetCalculatedStrength(user);
	target->Damage(dmg);
	std::println("{} attacks {} with Playful Cloud!", user->GetName(), target->GetName());
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
	return "Playful Cloud";
}
