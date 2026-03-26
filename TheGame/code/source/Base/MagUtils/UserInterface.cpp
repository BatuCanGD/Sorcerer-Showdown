#include "UIDisplay.h"
#include "Shikigami.h"
#include "Techniques.h"
#include "CursedTool.h"
#include "Specials.h"
#include "Domain.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

void UserInterface::ShowBattleEntry(const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
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

void UserInterface::DisplaySorcererStatus(Sorcerer* s) {
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
			std::println(" {}", s->GetTool()->GetName());
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
		if (s->GetSpecial() != nullptr)  std::print(" | 3 - Special [{}]", s->GetSpecial()->GetSpecialSimplifiedName());
		if (s->GetDomain() != nullptr || s->GetCounterDomain() != nullptr) std::print(" | \n4 - Domain actions");
		std::print(" | 5 - Taunt");
		if (!s->IsHeavenlyRestricted()) { std::print(" | 6 - RCT Usage | \n7 - Domain Amplification"); }
		if (!s->GetCursedTools().empty() || s->GetTool() != nullptr) { std::print(" | 8 - Cursed Tool"); }
		if (s->GetTechnique() != nullptr) { std::println(" | 9 - Technique Settings"); }
		if (!s->GetShikigami().empty()) { std::println("10 - Shikigami "); }

		std::print("\n=> ");
	}
}

void UserInterface::ClearScreen() {
	std::print("\033[2J\033[H");
}