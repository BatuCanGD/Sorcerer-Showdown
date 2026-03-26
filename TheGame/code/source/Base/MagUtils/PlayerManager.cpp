#include "PlayerManager.h"
#include "Shikigami.h"
#include "Techniques.h"
#include "CursedTool.h"
#include "Specials.h"
#include "Domain.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

void PlayerManager::OnPlayerTurn(Sorcerer& s, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
	int plrch = GetValidInput();
	switch (plrch) {
	case 1: {
		if (s.GetTechnique() == nullptr) {
			std::println("You don't have a technique to use!");
			break;
		}
		else if (s.GetTechnique()->BurntOut()) {
			std::println("Your technique is burnt out, you cant use it yet!");
			break;
		}
		Sorcerer* target = TargetSelector(battlefield, &s);

		if (target) {
			s.GetTechnique()->TechniqueMenu(&s, target);
		}
		break;
	}
	case 2: {
		if (Sorcerer* target = TargetSelector(battlefield, &s)) {
			std::println("{} engages in close combat with {}!", s.GetName(), target->GetName());
			s.Attack(target);
		}
		break;
	}
	case 3: {
		if (s.GetSpecial() == nullptr) {
			std::println("You dont have a Special move to use");
			return;
		}
		s.GetSpecial()->PerformSpecial(&s);
		break;
	}
	case 4: {
		PlayerDomainUsage(s);
		break;
	}
	case 5: {
		if (Sorcerer* target = TargetSelector(battlefield, &s)) {
			s.Taunt(target);
		}
		break;
	}
	case 6:
		if (s.IsHeavenlyRestricted()) {
			std::println("You cant use Reverse Cursed Technique because you are heavenly restricted!");
			return;
		}
		else if (s.GetCharacterMaxCE() < 500.0f) {
			std::println("You dont have enough cursed energy to continuously maintain Reverse Cursed Technique");
			return;
		}
		PlayerRCTusage(s);
		break;
	case 7:
		if (s.IsHeavenlyRestricted()) {
			std::println("You cant use Domain Amplification because you are heavenly restricted!");
			return;
		}
		PlayerDAusage(s);
		break;
	case 8:
		GetPlayerTools(s);
		break;
	case 9:
		s.GetTechnique()->TechniqueSetting(&s, battlefield);
		break;
	case 10:
		PlayerShikigami(s);
		break;
	default:
		std::println("Invalid Choice");
	}
}

void PlayerManager::PlayerDomainUsage(Sorcerer& s) {
	if (s.GetDomain() == nullptr && s.GetCounterDomain() == nullptr) {
		std::println("You dont have a domain and a counter to a domain");
		return;
	}
	if (s.GetDomain() != nullptr) {
		std::println("Domain Status: [{}]", s.DomainActive() ? "Active" : "Inactive");
	}
	if (s.GetCounterDomain() != nullptr) {
		std::println("{} Status: [{}]", s.GetCounterDomain()->GetDomainName(), s.CounterDomainActive() ? "Active" : "Inactive");
	}
	if (s.GetDomain() != nullptr) {
		std::print("1 - Activate Domain | 2 - Disable Domain ");
	}
	if (s.GetCounterDomain() != nullptr) {
		std::println("\n3 - Activate {} | 4 - Disable {} ", s.GetCounterDomain()->GetDomainName(), s.GetCounterDomain()->GetDomainName());
	}
	std::print("=> ");
	int ch = GetValidInput();
	switch (ch) {
	case 1:
		if (s.GetDomain() == nullptr) {
			std::println("You dont have a domain");
			break;
		}
		s.ActivateDomain();
		break;
	case 2:
		if (s.GetDomain() == nullptr) {
			std::println("You dont have a domain");
			break;
		}
		s.DeactivateDomain();
		break;
	case 3:
		if (s.GetCounterDomain() == nullptr) {
			std::println("You dont have a counter domain");
			break;
		}
		s.ActivateCounterDomain();
		break;
	case 4:
		if (s.GetCounterDomain() == nullptr) {
			std::println("You dont have a counter domain");
			break;
		}
		s.DeactivateCounterDomain();
		break;
	default:
		std::println("Invalid Input");
	}
}

void PlayerManager::GetPlayerTools(Sorcerer& s) {
	int count = 1;
	std::println("Available Tools:");
	for (const auto& tool : s.GetCursedTools()) {
		std::println("{} - {}", count++, tool->GetName());
	}

	if (s.GetTool() != nullptr) {
		std::println("\n0 - Unequip ({})", s.GetTool()->GetName());
	}
	else {
		std::println("\n0 - Go Back");
	}

	std::print("\n=> ");
	int choice = GetValidInput();
	s.CursedToolChoice(choice);
}

void PlayerManager::PlayerRCTusage(Sorcerer& s) {
	std::println("1-Enable RCT, 2-Boost RCT, 3-Disable RCT");
	int choice = GetValidInput();
	switch (choice) {
	case 1:
		s.EnableRCT();
		std::println("You have started using RCT");
		break;
	case 2:
		s.BoostRCT();
		std::println("You have started pumping RCT at maximum output");
		break;
	case 3:
		s.DisableRCT();
		std::println("You have disabled RCT");
		break;
	default:
		std::println("Invalid RCT Choice");
	}
}

void PlayerManager::PlayerDAusage(Sorcerer& s) {
	std::println("1-On, 2-Off\n=>");
	int choice = GetValidInput();

	switch (choice) {
	case 1:
		s.SetAmplification(true);
		break;
	case 2:
		s.SetAmplification(false);
		break;
	}
}

void PlayerManager::PlayerShikigami(Sorcerer& s) {
	if (s.GetShikigami().empty()) {
		std::println("You dont have any shikigami to use");
		return;
	}
	int count = 1;
	for (const auto& sh : s.GetShikigami()) {
		std::println("{}: {} ", count, sh->GetName());
		count++;
	}
	std::println("Choose the shikigami you'd like to use\n=> ");
	int ch = GetValidInput(); ch--;
	if (ch >= 0 && ch < s.GetShikigami().size()) {
		Shikigami* sk = s.ChooseShikigami(ch);
		std::println("Chosen Shikigami: {} | [{}]", sk->GetName(), sk->GetShikigamiStatus());
		if (!sk->IsActivePhysically()) {
			std::println("1 - Manifest");
		}
		if (!sk->IsPartiallyActive()) {
			std::println("2 - Partially Manifest");
		}
		if (sk->IsActive()) {
			std::println("3 - Dismiss");
		}
		std::println("0 - Cancel");
		std::print("=> ");

		int cs = GetValidInput();
		switch (cs) {
		case 1:
			if (sk->IsActivePhysically()) {
				std::println("{} is already physically present!", sk->GetName());
			}
			else {
				sk->Manifest();
				std::println("{} has been Physically Manifested on the battlefield!", sk->GetName());
			}
			break;
		case 2:
			if (sk->IsPartiallyActive()) {
				std::println("{} is already active!", sk->GetName());
			}
			else {
				sk->PartiallyManifest();
				std::println("{}'s technique is now Projected. [Ability Active]", sk->GetName());
			}
			break;
		case 3:
			if (!sk->IsActive()) {
				std::println("{} is already dormant.", sk->GetName());
			}
			else {
				sk->Withdraw();
				std::println("{} has been dismissed and is now Dormant.", sk->GetName());
			}
			break;
		case 0:
			std::println("Action cancelled.");
			break;

		default:
			std::println("Invalid input.");
			break;
		}

	}
	else {
		std::println("Invalid Choice");
	}
}

Sorcerer* PlayerManager::TargetSelector(const std::vector<std::unique_ptr<Sorcerer>>& battlefield, Sorcerer* player) {
	std::println("Choose your target:");
	for (size_t i = 0; i < battlefield.size(); ++i) {
		if (battlefield[i]->GetCharacterHealth() <= 0) continue;

		if (battlefield[i].get() == player) {
			std::println("{}: {} (You){}", i,
				battlefield[i]->GetName(),
				player->IsCharacterStunned() ? " (Stunned)" : "");
		}
		else {
			std::println("{}: {}{}", i,
				battlefield[i]->GetName(),
				battlefield[i]->IsCharacterStunned() ? " (Stunned)" : "");
		}
	}

	std::print("=> ");
	size_t t;
	if (!(std::cin >> t) || t >= battlefield.size() || battlefield[t].get() == player || battlefield[t]->GetCharacterHealth() <= 0.0) {
		if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(1000, '\n'); }
		std::println("Target missed or invalid!");
		return nullptr;
	}
	return battlefield[t].get();
}