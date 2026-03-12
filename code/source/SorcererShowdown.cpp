#include "Character.h"
#include "Sorcerer.h"
#include "CombatContext.h"
#include "Fighting.h"

import std;
using namespace std;

void GetSorcererTechnique(Sorcerer*, Character*);

int main() { // main
	vector<unique_ptr<Sorcerer>> battlefield;
	
	battlefield.push_back(make_unique<Sukuna>());
	battlefield.push_back(make_unique<Gojo>());
	battlefield.push_back(make_unique<test>());
	

	if (battlefield.size() < 2) {
		std::println("Not enough sorcerers to start the fight");
		return 1;
	}
	else if (battlefield.size() == 2) {
		std::println("Its just you and {}. Defeat him and win", battlefield[1]->GetName());
	}
	else if (battlefield.size() > 9) {
		std::println("You, {}, and {} more sorcerers are in the area. Defeat all of them to win", battlefield[1]->GetName(), battlefield.size() - 2);
	}
	else {
		std::println("You and {} more sorcerers are near you. Defeat them all to win", battlefield.size() - 1);
	}

	battlefield[0].get()->SetAsPlayer(true); // set the first sorcerer inserted as the player

	CombatContext context;
	FightActions fighting;

	std::println("-------The battle between {} sorcerers begin!-------", battlefield.size());
	std::println("-----------------------------------------------------");

	while (true) {

		for (const auto& s : battlefield) {

			if (s->GetCharacterHealth() <= 0.0) continue;

			if (s->IsThePlayer()) {
				std::println("-------Player's ({}'s) Turn-------", s.get()->GetName());
				std::println("Health: {}, Cursed Energy: {}", s.get()->GetCharacterHealth(), s.get()->GetCharacterCE());
				if (s->GetDomain() != nullptr) {
					std::println("Domain: {} [{}]",
						s->GetDomain()->GetDomainName(),
						s->DomainActive() ? "Active" : "Inactive");
				}
				if (s->GetTechnique() != nullptr) {
					std::println("Technique: {} [{}]",
						s->GetTechnique()->GetTechniqueName(),
						s->GetTechnique()->GetStringStatus());
				}
				std::println(" ");

				std::println("Choose action:");
				std::println("1-Use Technique, 2-Straight hands, 3-Use Special, 4-Domain, 5-Taunt");
				print("=> ");
				size_t plrch = 0; std::cin >> plrch;
				

				switch (plrch) {
				case 1: {
					GetSorcererTechnique(s.get(), battlefield[1].get());
					break;
				}
				case 2: {

					break;
				}
				case 3: {
					s->CheckSpecial(s.get());
					break;
				}
				case 4: {
					if (s.get()->GetDomain() == nullptr) break;

					if (s.get()->DomainActive()) {

						fighting.CheckDomain(s.get());
						break;
					}

					s.get()->ActivateDomain(s.get());
					std::println("Domain Expansion: {}", s.get()->GetDomain()->GetDomainName());

					break;
				}
				case 5: {
					context.Taunt(s.get(), battlefield[1].get());
					break;
				}
				default:
					std::println("Invalid Choice");
				}
				


				for (int i = 0; i < 2; i++) std::println(" ");
			}
			else {
				std::println("-------{}'s Turn-------", s->GetName());
				std::println("Health: {}, Cursed Energy: {}", s->GetCharacterHealth(), s->GetCharacterCE());
				if (s->GetDomain() != nullptr) {
					std::println("Domain: {} [{}]",
						s->GetDomain()->GetDomainName(),
						s->DomainActive() ? "Active" : "Inactive");
				}
				if (s->GetTechnique() != nullptr) {
					std::println("Technique: {} [{}]",
						s->GetTechnique()->GetTechniqueName(),
						s->GetTechnique()->GetStringStatus());
				}
				std::println(" ");

				s->OnSorcererTurn();
				s->CheckSpecial(s.get());

				for (int i = 0; i < 2; i++) std::println(" ");
			}
			std::cin.ignore();
		}

		//// DEFEATED CHARACTER REMOVAL ////
		auto [removed_begin, removed_end] = std::ranges::remove_if(battlefield, [](const auto& s) {
			if (s->GetCharacterHealth() <= 0.0) {
				std::println("{} has been defeated and is removed from the battlefield!", s->GetName());
				return true;
			}
			return false;
		});
		battlefield.erase(removed_begin, removed_end);


		//// DOMAIN EXPANSION CLASH TRIGGER AND CHECK ////
		vector<Sorcerer*> active_domains;
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
				s->GetDomain()->SetClashState(false);
			}
		}
		else if (active_domains.size() == 2) {
			for (const auto& s : active_domains) {
				s->GetDomain()->SetClashState(true);
			}
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
				if (s.get() == domain_user || s == nullptr) continue;
				std::println("{}'s domain; {} has hit {}", 
					domain_user->GetName(), 
					domain_user->GetDomain()->GetDomainName(), 
					s->GetName());

				domain_user->GetDomain()->OnSureHit(*s);
			}
		}

		std::println("press enter to Continue...");
		std::cin.ignore();


		/// CHECK IF PLAYER IS DEAD OR NOT /// DO OTHER STUFF ON THE SIDE
		bool player_found = false;
		for (const auto& c : battlefield) {
			if (c->IsThePlayer()) {
				player_found = true;
			}
			c->RegenCE();
		}
		//////// GAME ENDED // PLAYER DEAD // ALL SORCERERS DEAD //
		if (!player_found) {
			std::println("You have been defeated! Game Over.");
			break;
		}
		else if (battlefield.size() <= 1) {
			std::println("Congratulations! You have defeated all other sorcerers and won the battle!");
			break;
		}
		for (int i = 0; i < 20; i++) std::println(" ");
	}
	std::println("press enter to end the game...");
	std::cin.ignore();
	return 0;
}

void GetSorcererTechnique(Sorcerer* user, Character* target) {
	if (user->GetTechnique() == nullptr) return;

	int choice = 0;
	
	if (Limitless* limitless = dynamic_cast<Limitless*>(user->GetTechnique())) {
		std::println("1-use Blue, 2-use Red, 3-use Purple");
		print("=> ");

		std::cin >> choice;
		switch (choice) {
		case 1:
		case 2:
		case 3:
			limitless->UseTheLimitlessTechnique(static_cast<Limitless::LimitlessType>(choice), user, target);
			break;
		default:
			std::println("Invalid Choice");
		}


	}
	else if (Shrine* shrine = dynamic_cast<Shrine*>(user->GetTechnique())) {
		std::println("1-use Dismantle, 2-use Cleave");
		std::cin >> choice;

		switch (choice) {
		case 1:
		case 2:
			shrine->UseShrineTechnique(static_cast<Shrine::ShrineType>(choice), user, target);
			break;
		default:
			std::println("Invalid Choice");
		}
	}
	std::cin.ignore();
}
