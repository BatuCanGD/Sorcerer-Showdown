#include "BattleManager.h"
#include "CharacterList.h"
#include "DevFile.h"
#include "Limitless.h"
#include "ShikigamiList.h"
#include "DomainList.h"
#include "UIDisplay.h"
#include "Utils.h"


import std;

bool BattleManager::GameEndCheck(const std::vector<std::unique_ptr<Sorcerer>>& battlefield, bool spectator_mode) {
	int alive_sorcerers = 0;
	bool player_found = false;

	for (const auto& sc : battlefield) {
		if (sc->GetCharacterHealth() > 0) {
			alive_sorcerers++;
			if (sc->IsThePlayer()) player_found = true;
		}
	}
	if (spectator_mode && alive_sorcerers <= 1) return true;
	if (!spectator_mode && (!player_found || alive_sorcerers == 1)) return true;
	return false;
}

bool BattleManager::SkipTurnFullyCheck() {
	std::println("1 - Skip AI sorcerer turns\n"
				 "2 - Keep AI sorcerer turns");
	std::println("this is for if you want to read what every sorcerer on the board does line by line\n"
				 "or if you want to skip to do a game worth of simulation");
	std::print("=> ");
	int ch = GetValidInput();
	UserInterface::ClearScreen();
	switch (ch) {
	case 1: return true;
	case 2: return false;
	default: return false;
	}
}

bool BattleManager::SetupBattlefield(std::vector<std::unique_ptr<Sorcerer>>& battlefield, std::map<std::string, int>& sorcerer_counts) {
	bool choosing = true; bool spec_mode = false;
	int c = 0;

	while (choosing) {
		std::println("Choose your sorcerer and the amount of opponents you want to fight!");
		if (!spec_mode) {
			std::println("===> Player: {}", battlefield.empty() ? "None" : battlefield[0]->GetName());
		}
		else {
			std::println("[<Spectator Mode Enabled>]");
		}
		for (auto const& [name, count] : sorcerer_counts) {
			if (count > 0) std::println("{} x{}", name, count);
		}

		std::println("\n1 - Gojo | 2 - Sukuna | 3 - Toji Fushiguro |\n4-Yuta Okkotsu\n\n-2 - Spectator mode | -1 - Undo | 0 - Finish |");

		if (!(std::cin >> c)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
		else if (c == -2) {
			if (spec_mode) {
				spec_mode = false;
			}
			else {
				spec_mode = true;
			}
		}
		else {
			std::unique_ptr<Sorcerer> s = nullptr;
			switch (c) {
			case 1:   s = std::make_unique<Gojo>(); break;
			case 2:   s = std::make_unique<Sukuna>(); break;
			case 3:	  s = std::make_unique<Toji>(); break;
			case 4:	  s = std::make_unique<Yuta>(); break;
			case 2147: s = std::make_unique<test_sorcerer>(); break;
			}

			if (s) {
				sorcerer_counts[s->GetName()]++;
				battlefield.push_back(std::move(s));
			}
		}
		UserInterface::ClearScreen();
	}

	if (spec_mode) {
		return true;
	}
	else {
		return false;
	}
}

bool BattleManager::ManageEndOfTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield, bool spectator_mode) {
	std::println("\n\n============ END OF TURN SUMMARY ============");

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
		if (c->IsThePlayer() || spectator_mode) {
			player_alive = true;
		}

		c->CleanupShikigami();
		c->TickShikigami();

		double damage_taken = c->GetCharacterPreviousHealth() - c->GetCharacterHealth();
		if (damage_taken > 0) {
			std::println("{} took {:.1f} damage this turn", c->GetName(), damage_taken);
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
		c->RecoverBurnout();
		c->RecoverTechniqueBurnout(c->GetTechnique());
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

bool BattleManager::IsBattleOver(bool game_over ,bool player_found,bool spectator_mode, std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
	if (!player_found && battlefield.size() == 1 && !spectator_mode) {
		std::println("\nYou have been defeated by {}! Game Over.", battlefield[0]->GetName());
		return true;
	}
	else if (!player_found && !spectator_mode) {
		std::println("\nYou have been defeated! Game Over.");
		return true;
	}
	else if ((battlefield.size() == 1 || game_over) && !spectator_mode) {
		std::println("\nCongratulations! You have defeated all other sorcerers and won the battle!");
		return true;
	}
	else if (battlefield.size() == 1 && spectator_mode){
		std::println("The battle has ended, only one remains");
		return true;
	}
	return false;
}

