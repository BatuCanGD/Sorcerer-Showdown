#include "CombatContext.h"
#include "Character.h"
#include "Sorcerer.h"

void CombatContext::WorldCuttingSlashReady(Sorcerer* user) {
	Technique* currentTech = user->GetTechnique();
	Shrine* shrinePtr = dynamic_cast<Shrine*>(currentTech);

	if (shrinePtr == nullptr) return;

	const auto& shikigami_list = user->GetShikigami();
	for (const auto& s : shikigami_list) {
		if (Mahoraga* m = dynamic_cast<Mahoraga*>(s.get())) {
			if (m->FullyAdaptedToInfinity()) {
				shrinePtr->SetWCS(true);
				println("The blueprint is complete. World Cutting Slash enabled!");
				return;
			}
		}
	}
}