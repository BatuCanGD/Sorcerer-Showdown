#include "Specials.h"
#include "Character.h"
#include "Shikigami.h"
#include "Techniques.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

void WorldCuttingSlash::PerformSpecial(Sorcerer* user) {
	Technique* currentTech = user->GetTechnique();
	Shrine* shrinePtr = dynamic_cast<Shrine*>(currentTech);

	if (shrinePtr == nullptr) return;

	const auto& shikigami_list = user->GetShikigami();
	for (const auto& s : shikigami_list) {
		if (Mahoraga* m = dynamic_cast<Mahoraga*>(s.get())) {
			if (m->FullyAdapted()) {
				shrinePtr->SetWCS(true);
				std::println("The blueprint is complete. World Cutting Slash enabled!");
				return;
			}
			else {
				std::println("The blueprint is incomplete. World Cutting Slash cannot be used yet.");
			}
		}
	}
}
std::string WorldCuttingSlash::GetSpecialSimplifiedName() const {
	return "World Cutting Slash";
}