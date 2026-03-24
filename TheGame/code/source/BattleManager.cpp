#include "BattleManager.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

bool BattleManager::GameEndCheck(const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
	int alive_sorcerers = 0;
	bool player_alive = false;

	for (const auto& sc : battlefield) {
		if (sc->GetCharacterHealth() > 0) {
			if (sc->IsThePlayer()) player_alive = true;
			else alive_sorcerers++;
		}
	}

	if (!player_alive || alive_sorcerers == 0) {
		return true;
	}
	return false;
}

void BattleManager::ShowBattleEntry(const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
	if (battlefield.size() == 2) {
		std::println("Its just you and {}. Defeat them and win", battlefield[1]->GetName());
	}
	else if (battlefield.size() > 9) {
		std::println("You, {}, and {} more sorcerers are in the area. Defeat all of them to win", battlefield[1]->GetName(), battlefield.size() - 2);
	}
	else {
		std::println("You and {} more sorcerers are near you. Defeat them all to win", battlefield.size() - 1);
	}

	std::println("-------Let the battle between {} sorcerers begin!-------", battlefield.size());
	std::println("-------------------------------------------------------");
}

void BattleManager::OnPlayerTurn(Sorcerer& s, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
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
		else if (s.GetMaxCharCE() < 500.0f) {
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

void BattleManager::PlayerDomainUsage(Sorcerer& s) {
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

void BattleManager::GetPlayerTools(Sorcerer& s) {
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

void BattleManager::PlayerRCTusage(Sorcerer& s) {
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

void BattleManager::PlayerDAusage(Sorcerer& s) {
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

void BattleManager::PlayerShikigami(Sorcerer& s) {
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
		if (!sk->IsActive()) {
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
			if (sk->IsActive()) {
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

void BattleManager::SetupBattlefield(std::vector<std::unique_ptr<Sorcerer>>& battlefield, std::map<std::string, int>& sorcerer_counts) {
	bool choosing = true;
	int c = 0;

	while (choosing) {
		std::println("Choose your sorcerer and the amount of opponents you want to fight!");
		std::println("===> Player: {}", battlefield.empty() ? "None" : battlefield[0]->GetName());

		for (auto const& [name, count] : sorcerer_counts) {
			if (count > 0) std::println("{} x{}", name, count);
		}

		std::println("\n1 - Gojo | 2 - Sukuna | 3 - Toji Fushiguro |\n4-Yuta Okkotsu\n\n-1 - Undo | 0 - Finish |");

		if (!(std::cin >> c)) {
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			continue;
		}

		if (c == 0) {
			if (battlefield.size() < 2) {
				std::println("You need 2 or more sorcerers to start the fight!");
				std::cin.ignore();
				std::cin.get();
			}
			else {
				choosing = false;
			}
		}
		else if (c == -1 && !battlefield.empty()) {
			sorcerer_counts[battlefield.back()->GetName()]--;
			battlefield.pop_back();
		}
		else {
			std::unique_ptr<Sorcerer> s = nullptr;
			switch (c) {
			case 1:   s = std::make_unique<Gojo>(); break;
			case 2:   s = std::make_unique<Sukuna>(); break;
			case 3:	  s = std::make_unique<Toji>(); break;
			case 4:	  s = std::make_unique<Yuta>(); break;
			case 150: s = std::make_unique<test_sorcerer>(); break;
			}

			if (s) {
				sorcerer_counts[s->GetName()]++;
				battlefield.push_back(std::move(s));
			}
		}
		ClearScreen();
	}
}

void BattleManager::DisplaySorcererStatus(Sorcerer* s) {
	if (s->IsThePlayer()) {
		std::println("-------------Player's ({}'s) Turn-------------- {}", s->GetName(), s->IsCharacterStunned() ? "(Stunned)" : "");
	}
	else {
		std::println("-------------{}'s Turn-------------- {}", s->GetName(), s->IsCharacterStunned() ? "(Stunned)" : "");
	}

	std::print("Health: {} | ", s->GetCharacterHealth());

	if (!s->IsHeavenlyRestricted()) {
		std::print("Cursed Energy: {} | RCT: [{}] | DA: [{}]",
			s->GetCharacterCE(),
			s->GetRCTstatus(),
			s->GetDAstatus());
	}
	std::println("");

	if (s->GetDomain() != nullptr) {
		std::print("Domain: {} [{}]   ",
			s->GetDomain()->GetDomainName(),
			s->DomainActive() ? "Active" : "Inactive");
	}
	if (s->GetTechnique() != nullptr) {
		std::println("Technique: {} [{}]",
			s->GetTechnique()->GetTechniqueName(),
			s->GetTechnique()->GetStringStatus());
	}
	if (s->GetCounterDomain() != nullptr) {
		std::print("{}: [{}]   ", 
			s->GetCounterDomain()->GetDomainName(), 
			s->CounterDomainActive() ? "Active" : "Inactive");
	}


	if (!s->GetCursedTools().empty() || s->GetTool() != nullptr) {
		std::print("Inventory: ");
		if (s->GetCursedTools().empty()) {
			std::println("[Empty]");
		}
		else {
			for (const auto& t : s->GetCursedTools()) {
				std::print("[{}] ", t->GetName());
			}
			std::println("");
		}
		std::print("Current Tool: ");

		if (s->GetTool() != nullptr) {
			std::println(" {}",s->GetTool()->GetName());
		}
		else {
			std::println(" None");
		}
	}
	if (s->IsThePlayer() && s->IsCharacterStunned()) {
		std::println("\n\n");
		std::println("You have been Stunned and your turn has been skipped!");
		std::println("\n\n");
	}
	else if (s->IsThePlayer()) {
		std::println("\nChoose action:");
		if (s->GetTechnique() != nullptr) std::print("1 - Technique | ");
		std::print("2 - Fight");
		if (s->GetSpecial() != nullptr)  std::print(" | 3 - Special [{}]",s->GetSpecial()->GetSpecialSimplifiedName());
		if (s->GetDomain() != nullptr || s->GetCounterDomain() != nullptr) std::print(" | \n4 - Domain actions");
		std::print(" | 5 - Taunt");
		if (!s->IsHeavenlyRestricted()) { std::print(" | 6 - RCT Usage | \n7 - Domain Amplification"); }
		if (!s->GetCursedTools().empty() || s->GetTool() != nullptr) { std::println(" | 8 - Cursed Tool"); }
		if (s->GetTechnique() != nullptr) { std::println(" | 9 - Technique Settings"); }
		if (!s->GetShikigami().empty()) { std::println("10 - Shikigami "); }

		std::print("\n=> ");
	}
}

bool BattleManager::ManageEndOfTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
	auto [removed_begin, removed_end] = std::ranges::remove_if(battlefield, [](const auto& s) {
		if (s->GetCharacterHealth() <= 0.0) {
			std::println("{} has been defeated and is removed from the battlefield!", s->GetName());
			return true;
		}
		return false;
		});
	battlefield.erase(removed_begin, removed_end);

	bool player_alive = false;

	for (const auto& c : battlefield) {
		auto limitless = dynamic_cast<Limitless*>(c->GetTechnique());
		if (limitless) {
			limitless->InfinityNerf(c.get());
		}
		if (c->IsThePlayer()) {
			player_alive = true;
		}

		c->CleanupShikigami();
		c->TickShikigami();

		double damage_taken = c->GetCharacterPreviousHealth() - c->GetCharacterHealth();
		if (damage_taken > 0) {
			std::println("{} took {} damage this turn", c->GetName(), damage_taken);
			c->UseRCT();
			if (c->GetCharacterHealth() >= c->GetCharacterPreviousHealth()) {
				std::println("{} healed the damage back!", c->GetName());
			}
			else if (c->GetCharacterHealth() > (c->GetCharacterPreviousHealth() - damage_taken)) {
				std::println("{} partially healed their wounds.", c->GetName());
			}
		}
		else {
			c->UseRCT();
		}
		c->UpdatePreviousHP();
		c->RegenCE();
		c->ClearStunTime();
		c->RecoverBurnout(c->GetTechnique());
		c->TickZone();
	}
	return player_alive;
}

void BattleManager::DomainCheckAndPerform(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
	std::vector<Sorcerer*> active_domains;
	for (const auto& s : battlefield) {
		if (s->GetDomain() == nullptr) continue;
		if (s->DomainActive()) {
			active_domains.push_back(s.get());
		}
	}

	for (auto s : active_domains) {
		s->DomainDrain();
	}

	if (active_domains.size() > 2) {
		for (const auto& s : active_domains) {
			s->DeactivateDomain(); // dont forget / domain deactivation burns out the technique too
			s->GetDomain()->CollapseDomain();
		}
	}
	else if (active_domains.size() == 2) {
		for (const auto& s : active_domains) {
			s->GetDomain()->SetClashState(true);
		}
		active_domains[0]->GetDomain()->ClashDomains(*active_domains[0], *active_domains[1]);
	}
	else {
		for (const auto& s : active_domains) {
			if (s->GetDomain()->Clashing()) {
				s->GetDomain()->SetClashState(false);
			}
		}
	}
	///// DOMAIN EFFECTS, WITHOUT CLASH /////
	if (active_domains.size() == 1) {
		Sorcerer* domain_user = active_domains[0];

		for (const auto& s : battlefield) {
			if (s.get() == domain_user) continue;
			std::println("{} has been caught inside {}",
				s->GetName(),
				domain_user->GetDomain()->GetDomainName());
			domain_user->GetDomain()->OnSureHit(*s);
		}
	}
	for (const auto& s : battlefield) {
		s->TickDomain();
	}
}

Sorcerer* BattleManager::TargetSelector(const std::vector<std::unique_ptr<Sorcerer>>& battlefield, Sorcerer* player) {
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

bool BattleManager::IsBattleOver(bool game_over ,bool player_found, std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
	if (!player_found && battlefield.size() == 1) {
		std::println("\nYou have been defeated by {}! Game Over.", battlefield[0]->GetName());
		return true;
	}
	else if (!player_found) {
		std::println("\nYou have been defeated! Game Over.");
		return true;
	}
	else if (battlefield.size() == 1 || game_over) {
		std::println("\nCongratulations! You have defeated all other sorcerers and won the battle!");
		return true;
	}
	return false;
}

void BattleManager::ClearScreen() {
	std::print("\033[2J\033[H");
}