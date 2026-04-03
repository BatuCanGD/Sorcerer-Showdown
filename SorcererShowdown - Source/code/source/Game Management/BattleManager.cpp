#include "BattleManager.h"
#include "CharacterList.h"
#include "DevFile.h"
#include "Limitless.h"
#include "ShikigamiList.h"
#include "DomainList.h"
#include "UIDisplay.h"
#include "Utils.h"

import std;

bool BattleManager::GameEndCheck(const std::vector<std::unique_ptr<Character>>& battlefield, bool spectator_mode) {
	int alive_sorcerers = 0;
	bool player_found = false;

	for (const auto& sc : battlefield) {
		if (sc->GetCharacterHealth() > 0) {
			alive_sorcerers++;
			if (sc->IsThePlayer()) player_found = true;
		}
	}

	if (!spectator_mode && !player_found) return true;
	if (alive_sorcerers <= 1) return true;
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

bool BattleManager::SetupBattlefield(std::vector<std::unique_ptr<Character>>& battlefield, std::map<std::string, int>& sorcerer_counts) {
	bool choosing = true; bool spec_mode = false;
	int c = 0;

	std::vector<std::unique_ptr<Character>> characters;
	characters.push_back(std::make_unique<Gojo>());
	characters.push_back(std::make_unique<Sukuna>());
	characters.push_back(std::make_unique<Yuta>());
	characters.push_back(std::make_unique<Toji>());
	characters.push_back(std::make_unique<test_sorcerer>());
	
	Character::ResetGlobalID();

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
		std::println("\n");
		int i = 1;
		for (const auto& s : characters) {
			std::println("{}: {}",i, s->GetName());
			i++;
		}
		std::println("-2 - Spectator mode | -1 - Undo | 0 - Finish |");

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
			Character::AddGlobalID(-1);
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
			if (c > 0 && c <= characters.size()) {
				int index = c - 1;
				std::unique_ptr<Character> new_character = characters[index]->Clone();
				sorcerer_counts[new_character->GetName()]++;
				battlefield.push_back(std::move(new_character));
			}
			else {
				std::println("Invalid selection!");
			}
		}
		UserInterface::ClearScreen();
	}
	return spec_mode;
}

bool BattleManager::ManageEndOfTurn(std::vector<std::unique_ptr<Character>>& battlefield, bool spectator_mode) {
	std::println("{}=============== TURN AFTERMATH ==============={}", Color::BrightRed, Color::Clear);

	auto [removed_begin, removed_end] = std::ranges::remove_if(battlefield, [](const auto& s) {
		if (s->GetCharacterHealth() <= 0.0) {
			std::println("{}{} has been defeated and is removed from the battlefield!{}", Color::Red, s->GetNameWithID(), Color::Clear);
			return true;
		}
		return false;
		});
	battlefield.erase(removed_begin, removed_end);

	bool player_alive = false;

	for (const auto& c : battlefield) {
		if (auto sorc = dynamic_cast<Sorcerer*>(c.get())) {
			if (auto limitless = dynamic_cast<Limitless*>(sorc->GetTechnique())) {
				limitless->InfinityNerf(sorc);
			}
			sorc->CleanupShikigami();
			sorc->TickShikigami();
			sorc->RecoverBurnout();
			sorc->RecoverTechniqueBurnout(sorc->GetTechnique());
			sorc->TickZone();
			sorc->UseRCT();
		}

		double damage_taken = c->GetCharacterPreviousHealth() - c->GetCharacterHealth();
		if (damage_taken > 0) {
			std::println("{} took {}{:.1f} damage{} this turn", c->GetNameWithID(), Color::Red, damage_taken, Color::Clear);
			if (c->GetCharacterHealth() >= c->GetCharacterPreviousHealth()) {
				std::println("{} {}healed the damage back!{}", c->GetNameWithID(), Color::Green, Color::Clear);
			}
			else if (c->GetCharacterHealth() > (c->GetCharacterPreviousHealth() - damage_taken)) {
				std::println("{} {}partially healed their wounds.{}", c->GetNameWithID(), Color::Yellow, Color::Clear);
			}
		}
		else {
		}

		if (c->IsThePlayer() || spectator_mode) {
			player_alive = true;
		}

		c->UpdatePreviousHP();
		c->RegenCE();
		c->ClearStunTime();
		c->TickReinforcement();
	}
	std::println("{}======================================================={}", Color::Yellow, Color::Clear);
	return player_alive;
}

void BattleManager::DomainCheckAndPerform(std::vector<std::unique_ptr<Character>>& battlefield) {
	std::println("\n\n{}================= END OF TURN SUMMARY ================={}", Color::Yellow, Color::Clear); // this is here now because its just 1 line away from manage end of turn
	std::println("{}============= DOMAINS AND CLASHES ============{}", Color::BrightMagenta, Color::Clear);
	std::vector<Sorcerer*> active_domains;

	for (const auto& s : battlefield) {
		if (auto sorcerers = dynamic_cast<Sorcerer*>(s.get())) {
			if (sorcerers->GetDomain() == nullptr) continue;
			if (sorcerers->DomainActive()) {
				active_domains.push_back(sorcerers);
			}
		}
	}

	for (auto s : active_domains) {
		s->DomainDrain();
	}

	if (active_domains.size() > 2) {
		for (const auto& s : active_domains) {
			s->DeactivateDomain(); 
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
		if (auto sr = dynamic_cast<Sorcerer*>(s.get())) {
			sr->TickDomain();
		}
	}
}

bool BattleManager::IsBattleOver(bool game_over ,bool player_found,bool spectator_mode, std::vector<std::unique_ptr<Character>>& battlefield) {
	if (!game_over && battlefield.size() > 1 && (player_found || spectator_mode)) return false;

	if (battlefield.empty()) {
		if (spectator_mode) {
			std::println("Every sorcerer has been wiped off the battlefield!");
		}
		else {
			std::println("You and everyone else has been wiped off the battlefield, it's a draw!");
		}
		return true;
	}
	if (spectator_mode) {
		if (battlefield.size() == 1) {
			std::println("The battle has ended, {} is the last one standing!", battlefield[0]->GetNameWithID());
		}
		return true;
	}
	if (!player_found) {
		if (battlefield.size() == 1) {
			std::println("\nYou have been defeated by {}! Game Over.", battlefield[0]->GetNameWithID());
		}
		else {
			std::println("\nYou have been defeated! The battle rages on without you. Game Over.");
		}
		return true;
	}
	if (player_found && battlefield.size() == 1) {
		std::println("\nCongratulations! You have defeated all other sorcerers and won the battle!");
		return true;
	}
	return false;
}

