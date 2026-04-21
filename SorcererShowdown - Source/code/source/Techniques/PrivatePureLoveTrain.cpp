#include "PrivatePureLoveTrain.h"
#include "IdleDeathGamble.h"
#include "Character.h"
#include "CurseUser.h"
#include "Utils.h"

import std;

bool PrivatePureLoveTrain::PlinkoUsed() const {
	return plinko_used;
}
void PrivatePureLoveTrain::SetPlinkoStatus(bool) {
	plinko_used = false;
}
void PrivatePureLoveTrain::TickPlinkoCooldown() {
	if (plinko_used) {
		plinko_cooldown_time++;
		if (plinko_cooldown_time >= max_plinko_cooldown_time) {
			plinko_used = false;
			plinko_cooldown_time = 0;
		}
	}
}

std::string PrivatePureLoveTrain::GetTechniqueName() const {
	return "\033[92mPrivate Pure Love Train\033[0m";
}
std::string PrivatePureLoveTrain::GetTechniqueSimpleName() const {
	return "Private Pure Love Train";
}

void PrivatePureLoveTrain::UsePlinkoBalls(CurseUser* user, Character* target) {
	if (plinko_used) return;
	int pplt = GetRandomNumber(1, 239);
	if (pplt >= 200) {
		target->Damage(GOLDEN_PLINKO_DAMAGE);
		std::println("{} stuns {} with a Golden Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	else if (pplt >= 100) {
		target->Damage(GREEN_PLINKO_DAMAGE);
		std::println("{} stuns {} with a Green Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	else {
		target->Damage(RED_PLINKO_DAMAGE);
		std::println("{} stuns {} with a Red Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	plinko_used = true;
	target->SetStunState(true);
}
void PrivatePureLoveTrain::UseShutterDoors(CurseUser* user, Character* target) {
	int pplt = GetRandomNumber(1, 239);
	if (pplt >= 200) {
		target->Damage(GOLDEN_SHUTTER_DAMAGE);
		std::println("{} hits {} with a Golden Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
	else if (pplt >= 100) {
		target->Damage(GREEN_SHUTTER_DAMAGE);
		std::println("{} hits {} with a Green Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
	else {
		target->Damage(RED_SHUTTER_DAMAGE);
		std::println("{} hits {} with a Red Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
}
void PrivatePureLoveTrain::UseJackpotRush(CurseUser* user, Character* target) {
	auto idg = dynamic_cast<IdleDeathGamble*>(user->GetDomain());
	if (idg && idg->HasHitJackpot()) {
		target->Damage(user->GetBaseAttackDamage() * 2.0);
		std::println("{} hits {} with a volley of jackpot boosted rush attacks!", user->GetNameWithID(), target->GetNameWithID());
	}
	else {
		std::println("you arent able to access this normally");
	}
}

void PrivatePureLoveTrain::Chant() {}

void PrivatePureLoveTrain::TechniqueMenu(CurseUser* user, Character* target, Battlefield&) {
	if (user->DomainAmplificationActive()) {
		std::println("You cannot use your innate technique due to domain amplification!");
		return;
	}
	auto idg = dynamic_cast<IdleDeathGamble*>(user->GetDomain());
	if (idg && idg->HasHitJackpot()) {
		std::println("1 - Use Plinko balls | 2 - Use Shutter doors || 3 - {}Use Jackpot Rush{}", Color::Green, Color::Clear);
	}
	else {
		std::println("1 - Use Plinko balls | 2 - Use Shutter doors");
	}
	std::print("=> ");
	int choice = GetValidInput();

	switch (choice) {
	case 1:
		UsePlinkoBalls(user, target);
		break;
	case 2:
		UseShutterDoors(user, target);
		break;
	case 3:
		if (idg && idg->HasHitJackpot()) {
			UseJackpotRush(user, target);
		}
		else {
			std::println("Invalid Input");
		}
		break;
	default:
		std::println("Invalid Input");
	}
}

void PrivatePureLoveTrain::TechniqueSetting(CurseUser*, Battlefield&) {}

std::unique_ptr<Technique> PrivatePureLoveTrain::Clone() const {
	return std::make_unique<PrivatePureLoveTrain>();
}