#include "BattleManager.h"
#include "Sorcerer.h"
#include "CombatContext.h"

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

void BattleManager::OnPlayerTurn(Sorcerer& s, const std::vector<std::unique_ptr<Sorcerer>>& battlefield, CombatContext& context) {
	size_t plrch = 0; std::cin >> plrch;
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
		s.CheckSpecial(&s);
		break;
	}
	case 4: {
		if (s.GetDomain() == nullptr) {
			std::println("You dont have a domain to use!");
			break;
		}
		s.ActivateDomain();
		break;
	}
	case 5: {
		if (Sorcerer* target = TargetSelector(battlefield, &s)) {
			context.Taunt(&s, target);
		}
		break;
	}
	case 6:
		PlayerRCTusage(s);
		break;
	case 7:
		PlayerDAusage(s);
		break;
	case 8:
		GetPlayerTools(s);
		break;
	default:
		std::println("Invalid Choice");
	}
}

void BattleManager::GetPlayerTools(Sorcerer& s) {
	int count = 1;
	for (const auto& tool : s.GetCursedTools()) {
		std::println("{} - {}", count++, tool->GetName());
	}
	if (s.GetTool() != nullptr) {
		std::println("\n0 - Unequip");
	}

	int choice = 0; std::cin >> choice;
	s.CursedToolChoice(choice);
}

void BattleManager::PlayerRCTusage(Sorcerer& s) {
	std::println("1-Enable RCT, 2-Boost RCT, 3-Disable RCT");
	int choice = 0; std::cin >> choice;
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
	int choice = 0; std::cin >> choice;


	switch (choice) {
	case 1:
		s.SetAmplification(true);
		break;
	case 2:
		s.SetAmplification(false);
		break;
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

		std::println("\n1 - Gojo | 2 - Sukuna | 3 - Toji Fushiguro |\n\n-1 - Undo | 0 - Finish |");

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

	if (s->IsThePlayer()) {
		std::println("\nChoose action:");
		if (s->GetTechnique() != nullptr) std::print("1-Technique | ");
		std::print("2-Fight");

		if (s->GetSpecial() != nullptr)  std::print(" | 3-Special");
		if (s->GetDomain() != nullptr)   std::print(" | 4-Domain");

		std::print(" | 5-Taunt");

		if (!s->IsHeavenlyRestricted()) {
			std::print(" | 6-RCT Usage | 7-Domain Amplification");
		}
		if (!s->GetCursedTools().empty() || s->GetTool() != nullptr) {
			std::println(" | 8-Cursed Tool");
		}

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
			std::println("{}'s domain; {} has hit {}",
				domain_user->GetName(),
				domain_user->GetDomain()->GetDomainName(),
				s->GetName());

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