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

	std::print("Health: {:.1f} | ", s->GetCharacterHealth());

	if (!s->IsHeavenlyRestricted()) {
		std::print("Cursed Energy: {:.1f} | RCT: [{}] | DA: [{}]",
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

		Domain* domain = s->GetDomain();
		Domain* counter = s->GetCounterDomain();
		Technique* tech = s->GetTechnique();
		Specials* special = s->GetSpecial();

		std::string techdisplay = (tech == nullptr) ? "1 - Technique [None] " : std::format("1 - Technique [{}] ", tech->GetTechniqueName());
		std::string fightdisplay = "2 - Fight";
		std::string specdisplay = (special == nullptr) ? "3 - Special [None] " : std::format("3 - Special [{}] ", special->GetSpecialSimplifiedName());
		std::string domaindisplay = (domain == nullptr && counter == nullptr) ? "4 - Domain [Locked]" : "4 - Domain Actions";
		std::string tauntdisplay = "5 - Taunt";
		std::string rctdisplay = s->IsHeavenlyRestricted() ? "6 - Reverse Cursed Technique [Locked]" : std::format("6 - Reverse Cursed Technique [{}]", s->GetRCTstatus());
		std::string ampdisplay = s->IsHeavenlyRestricted() ? "7 - Domain Amplification [Locked]" : std::format("7 - Domain Amplification [{}]", s->GetDAstatus());		
		std::string tooldisplay = (s->GetCursedTools().empty() && s->GetTool() == nullptr) ? "8 - Cursed Tools [None]" : "8 - Cursed Tools";
		std::string setdisplay = (tech == nullptr) ? "9 - Technique Settings [Locked]" : "9 - Technique Settings";
		std::string shikigamidisplay = (s->GetShikigami().empty()) ? "10 - Shikigami [None]" : "10 - Shikigami";


		std::println("  {:<35} | {:<35} | {:<35} ", techdisplay, fightdisplay, specdisplay);
		std::println("  {:<35} | {:<35} | {:<35} ", domaindisplay, tauntdisplay, rctdisplay);
		std::println("  {:<35} | {:<35} | {:<35} ", ampdisplay, tooldisplay, setdisplay);
		std::println("  {:<35} |", shikigamidisplay);
		

		std::print("\n=> ");
	}
}

void UserInterface::ClearScreen() {
	std::print("\033[2J\033[H");
}