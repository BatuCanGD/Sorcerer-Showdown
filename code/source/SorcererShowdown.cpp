#include "Character.h"
#include "Sorcerer.h"
#include "CombatContext.h"
#include "Fighting.h"

#include <print>

import std;
using namespace std;

int main() { // main
	vector<unique_ptr<Sorcerer>> battlefield;
	battlefield.push_back(make_unique<Sukuna>());
	battlefield.push_back(make_unique<Gojo>());
	battlefield.push_back(make_unique<test>());
	
	battlefield[0].get()->SetAsPlayer(true);

	CombatContext context;
	FightActions fighting;
	if (battlefield.size() < 2) {
		println("Not enough sorcerers to start the fight");
		return 1;
	}
	else if (battlefield.size() == 2) {
		println("Its just you and {}. Defeat him and win", battlefield[1]->GetName());
	}
	else if (battlefield.size() > 9) {
		println("You, {}, {} and {} more sorcerers are in the area. Defeat all of them to win", battlefield[1]->GetName(), battlefield[2]->GetName(), battlefield.size() - 3);
	}
	else {
		println("You, {} and {} more sorcerers are near you. Defeat them all to win", battlefield[1]->GetName(), battlefield.size() - 2);
	}
	
	println("-------The battle between {} sorcerers begin!-------", battlefield.size());
	println("-----------------------------------------------------");

	
	while (true) {

		for (const auto& s : battlefield) {

			if (s->GetCharacterHealth() <= 0.0) continue;

			if (s->IsThePlayer()) {
				println("-------Player's ({}'s) Turn-------", s.get()->GetName());
				println("Health: {}, Cursed Energy: {}", s.get()->GetCharacterHealth(), s.get()->GetCharacterCE());
				if (s->GetDomain() != nullptr) {
					println("Domain: {} [{}]",
						s->GetDomain()->GetDomainName(),
						s->IsDomainActive() ? "Active" : "Inactive");
				}
				if (s->GetTechnique() != nullptr) {
					println("Technique: {} [{}]",
						s->GetTechnique()->GetTechniqueName(),
						s->GetTechnique()->GetStringStatus());
				}
				println(" ");

				println("player is the biggest fraud");

				for (int i = 0; i < 2; i++) println(" ");
			}
			else {
				println("-------{}'s Turn-------", s->GetName());
				println("Health: {}, Cursed Energy: {}", s->GetCharacterHealth(), s->GetCharacterCE());
				if (s->GetDomain() != nullptr) {
					println("Domain: {} [{}]",
						s->GetDomain()->GetDomainName(),
						s->IsDomainActive() ? "Active" : "Inactive");
				}
				if (s->GetTechnique() != nullptr) {
					println("Technique: {} [{}]",
						s->GetTechnique()->GetTechniqueName(),
						s->GetTechnique()->GetStringStatus());
				}
				println(" ");

				s->OnSorcererTurn();

				for (int i = 0; i < 2; i++) println(" ");
			}
		}

		auto [removed_begin, removed_end] = ranges::remove_if(battlefield, [](const auto& s) {
			if (s->GetCharacterHealth() <= 0.0) {
				println("{} has been defeated and is removed from the battlefield!", s->GetName());
				return true;
			}
			return false;
		});
		battlefield.erase(removed_begin, removed_end);

		println("press enter to Continue...");
		cin.ignore();

		bool player_found = false;
		for (const auto& c : battlefield) {
			if (c.get()->IsThePlayer()) {
				player_found = true;
				break;
			}
		}

		if (!player_found) {
			println("You have been defeated! Game Over.");
			break;
		}
		else if (battlefield.size() <= 1) {
			println("Congratulations! You have defeated all other sorcerers and won the battle!");
			break;
		}
		for (int i = 0; i < 20; i++) println(" ");
	}
	println("press enter to end the game...");
	cin.ignore();
	return 0;
}
