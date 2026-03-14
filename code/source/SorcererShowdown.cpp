#include "Character.h"
#include "Sorcerer.h"
#include "CombatContext.h"
#include "Fighting.h"

import std;

void ShowBattleEntry(const std::vector<std::unique_ptr<Sorcerer>>& battlefield);
void SetupBattlefield(std::vector<std::unique_ptr<Sorcerer>>& battlefield, std::map<std::string, int>& sorcerer_counts);
void OnPlayerTurn(Sorcerer& s, const std::vector<std::unique_ptr<Sorcerer>>& battlefield, FightActions& fighting, CombatContext& context);
Sorcerer* TargetSelector(const std::vector<std::unique_ptr<Sorcerer>>&, Sorcerer*);
bool CleanupSorcerers(std::vector<std::unique_ptr<Sorcerer>>&);
void DisplaySorcererStatus(Sorcerer* s);
void DomainCheckAndPerform(std::vector<std::unique_ptr<Sorcerer>>& battlefield);


int main() { // main
	std::vector<std::unique_ptr<Sorcerer>> battlefield;
	std::map <std::string, int> sorcerer_counts;

	SetupBattlefield(battlefield, sorcerer_counts);
	ShowBattleEntry(battlefield);
	
	battlefield[0].get()->SetAsPlayer(true); // set the first sorcerer inserted as the player

	CombatContext context;
	FightActions fighting;

	while (true) {
		for (const auto& s : battlefield) {
			if (s->GetCharacterHealth() <= 0.0) continue;

			if (s->IsThePlayer()) {
				DisplaySorcererStatus(s.get());
				OnPlayerTurn(*s, battlefield, fighting, context);
				
				for (int i = 0; i < 2; i++) std::println(" ");
			}
			else {
				DisplaySorcererStatus(s.get());

				s->OnSorcererTurn();
				s->CheckSpecial(s.get());

				for (int i = 0; i < 2; i++) std::println(" ");
			}
			std::cin.ignore();
		}

		bool player_found = CleanupSorcerers(battlefield);
		DomainCheckAndPerform(battlefield);

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

void ShowBattleEntry(const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
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
	std::println("--------------------------------------------------------");
}

void OnPlayerTurn(Sorcerer& s, const std::vector<std::unique_ptr<Sorcerer>>& battlefield,FightActions& fighting,CombatContext& context){
	size_t plrch = 0; std::cin >> plrch;
	switch (plrch) {
	case 1: {
		if (s.GetTechnique() == nullptr) {
			std::println("You don't have a technique to use!");
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
			fighting.Attack(&s, target);
		}	
		break;
	}
	case 3: {
		s.CheckSpecial(&s);
		break;
	}
	case 4: {
		if (s.GetDomain() == nullptr) {
			std::println("You dont have a domain to use!");
			break;
		}
		if (s.DomainActive()) {
			fighting.CheckDomain(&s);
			break;
		}
		s.ActivateDomain(&s);
		break;
	}
	case 5: {
		if (Sorcerer* target = TargetSelector(battlefield, &s)) {
			context.Taunt(&s, target);
		}
		break;
	}
	default:
		std::println("Invalid Choice");
	}
}

void SetupBattlefield(std::vector<std::unique_ptr<Sorcerer>>& battlefield, std::map<std::string, int>& sorcerer_counts) {
	bool choosing = true;
	int c = 0;

	while (choosing) {
		std::println("Choose your sorcerer and the amount of opponents you want to fight!");
		std::println("===> Player: {}", battlefield.empty() ? "None" : battlefield[0]->GetName());

		for (auto const& [name, count] : sorcerer_counts) {
			if (count > 0) std::println("{} x{}", name, count);
		}

		std::println("\n1 - Gojo / 2 - Sukuna / -1 - Undo / 0 - Finish");

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
			case 151: s = std::make_unique<test_sorcerer>(); break;
			}

			if (s) {
				sorcerer_counts[s->GetName()]++;
				battlefield.push_back(std::move(s));
			}
		}
		system("cls");
	}
}

void DisplaySorcererStatus(Sorcerer* s) {
	if (s->IsThePlayer()) {
		std::println("-------Player's ({}'s) Turn------- {}", s->GetName(), s->IsCharacterStunned() ? "(Stunned)" : "");
	}
	else {
		std::println("-------{}'s Turn------- {}", s->GetName(), s->IsCharacterStunned() ? "(Stunned)" : "");
	}

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
	if (s->IsThePlayer()) {
		std::println("Choose action:");
		std::println("1-Use Technique, 2-Straight hands, 3-Use Special, 4-Domain, 5-Taunt");
		std::print("=> ");
	}
}

bool CleanupSorcerers(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
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
		if (c->IsThePlayer()) {
			player_alive = true;
		}
		c->RegenCE();
	}
	return player_alive;
}

void DomainCheckAndPerform(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
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
}

Sorcerer* TargetSelector(const std::vector<std::unique_ptr<Sorcerer>>& battlefield, Sorcerer* player) {
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
	if (!(std::cin >> t) || t >= battlefield.size() || battlefield[t].get() == player) {
		if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(1000, '\n'); }
		std::println("Target missed or invalid!");
		return nullptr;
	}
	return battlefield[t].get();
}