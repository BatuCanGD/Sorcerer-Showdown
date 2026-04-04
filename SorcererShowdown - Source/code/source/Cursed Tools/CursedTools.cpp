#include "CursedTool.h"
#include "PhysicallyGifted.h"

double CursedTool::GetCalculatedStrength(Character* user)  const {
	if (user->IsPhysicallyGifted()) {
		auto pg = static_cast<PhysicallyGifted*>(user);
		return base_tool_damage + (pg->GetStrengthDamage() * 1.25);
	}
	else {
		return base_tool_damage + (user->GetCharacterMaxHealth() / 10.0);
	}
}