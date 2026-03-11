#include "CombatContext.h"
#include "Character.h"
#include "Sorcerer.h"
#include "Fighting.h"

#include <print>
#include <random>


void WorldCuttingSlash::WorldCuttingSlashReady(Sorcerer* user) {
	Technique* currentTech = user->GetTechnique();
	Shrine* shrinePtr = dynamic_cast<Shrine*>(currentTech);

	if (shrinePtr == nullptr) return;

	const auto& shikigami_list = user->GetShikigami();
	for (const auto& s : shikigami_list) {
		if (Mahoraga* m = dynamic_cast<Mahoraga*>(s.get())) {
			if (m->FullyAdaptedToInfinity()) {
				shrinePtr->SetWCS(true);
				std::println("The blueprint is complete. World Cutting Slash enabled!");
				return;
			}
		}
	}
}

void CombatContext::Taunt(Sorcerer* taunter, Character* taunted) {
	if (!taunter || !taunted) return;
	const double healthy_threshold = 0.70;
	const double injured_threshold = 0.40;
	const double critical_threshold = 0.20;

	int taunt_type = GetRandomNumber(1, 4);

	if (taunter->GetCharacterHealth() > (taunter->GetCharacterMaxHealth() * healthy_threshold)) {
		switch (taunt_type) {
		case 1:
			std::println("I'm surprised you've even managed to scratch me this much {}!", taunted->GetName());
			break;
		case 2:
			std::println("Is that all you've got, {}? I expected more from you!", taunted->GetName());
			break;
		case 3:
			std::println("You're not even worth my time, {}!", taunted->GetName());
			break;
		default:
			std::println("You should just give up now, {}!", taunted->GetName());
		}
	}
	else if (taunter->GetCharacterHealth() > (taunter->GetCharacterMaxHealth() * injured_threshold)){
		switch (taunt_type) {
		case 1:
			std::println("You're starting to annoy me, {}. Keep it up and I'll make you regret it!", taunted->GetName());
			break;
		case 2:
			std::println("You're not doing too bad, {}. But don't get too confident just yet!", taunted->GetName());
			break;
		case 3:
			std::println("Huh, you're actually putting up a fight, {}. I might have to take you more seriously!", taunted->GetName());
			break;
		default:
			std::println("You're not bad, {}. But I'm still better!", taunted->GetName());

		}
	}
	else if (taunter->GetCharacterHealth() > (taunter->GetCharacterMaxHealth() * critical_threshold)) {
		switch (taunt_type) {
		case 1:
			std::println("You're really starting to piss me off, {}. I'll make you regret your actions!", taunted->GetName());
			break;
		case 2:
			std::println("You're actually pretty strong, but it won't be enough to defeat me, {}!", taunted->GetName());
			break;
		case 3:
			std::println("You're really starting to get on my nerves, {}. I might have to end this quickly!", taunted->GetName());
			break;
		default:
			std::println("You think a few hits will stop me, {}? Im just getting warmed up!", taunted->GetName());
		}
	}
	else {
		switch (taunt_type){
		case 1:
			std::println("You think this is over, {}? I'll drag you to the grave with me!", taunted->GetName());
			break;
		case 2:
			std::println("Blood for blood, {}! You won't leave this place alive!", taunted->GetName());
			break;
		case 3:
			std::println("You're really starting to piss me off, {}. I might have to end this quickly!", taunted->GetName());
			break;
		default:
			std::println("I'll make you wish you were never born {}!", taunted->GetName());
		}
	}
}

int CombatContext::GetRandomNumber(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}