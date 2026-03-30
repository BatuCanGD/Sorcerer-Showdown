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
		std::println("-------------Player's ({}'s) Turn-------------- {}", s->GetNameWithID(), s->IsCharacterStunned() ? "(Stunned)" : "");
	}
	else {
		std::println("-------------{}'s Turn-------------- {}", s->GetNameWithID(), s->IsCharacterStunned() ? "(Stunned)" : "");
	}
	double hp = s->GetCharacterHealth();
	double max_hp = s->GetCharacterMaxHealth();
	std::string hp_color = Color::Green;

	if (!s->HPMoreThanMax(0.10)) {
		hp_color = Color::Red;
	}
	else if (!s->HPMoreThanMax(0.33)) {
		hp_color = Color::BrightRed;
	}
	else if (!s->HPMoreThanMax(0.66)) {
		hp_color = Color::Yellow;
	}
	else {
		hp_color = Color::Green;
	}
	

	std::print("Health: [{}{:.1f}/{:.1f}{}] | ",hp_color, hp, max_hp, Color::Clear);
	
	if (!s->IsHeavenlyRestricted()) {
		double ce = s->GetCharacterCE();
		double max_ce = s->GetCharacterMaxCE();
		std::string ce_color = Color::Cyan;

		if (!s->CEMoreThanMax(0.10)) {
			ce_color = Color::DimGray;
		}
		else {
			ce_color = Color::Cyan;
		}

		std::print("Cursed Energy: [{}{:.1f}/{:.1f}{}] | RCT: [{}] | DA: [{}]",
			ce_color, ce, max_ce, Color::Clear, s->GetRCTstatus(), s->GetDAstatus());
	}

	std::println("");

	if (s->GetDomain() != nullptr) {
		std::print("Domain: [{}] [{}]   ",
			s->GetDomain()->GetDomainName(),
			s->GetDomainStatus());
	
	}
	if (s->GetTechnique() != nullptr) {
		std::println("Technique: [{}] [{}]",
			s->GetTechnique()->GetTechniqueName(),
			s->GetTechnique()->GetStringStatus());
	}
	if (s->GetCounterDomain() != nullptr) {
		std::print("Counter: {} [{}]    ",
			s->GetCounterDomain()->GetDomainName(),
			s->GetCounterStatus());
	}

	if (!s->GetCursedTools().empty() || s->GetTool() != nullptr) {
		std::print("Inventory: ");
		if (s->GetCursedTools().empty()) {
			std::println("[{}Empty{}]",Color::DimGray,Color::Clear);
		}
		else {
			for (const auto& t : s->GetCursedTools()) {
				std::print("[{}] ", t->GetName());
			}
			std::println("");
		}
		std::print("Current Tool");

		if (s->GetTool() != nullptr) {
			std::println(" [{}]", s->GetTool()->GetName());
		}
		else {
			std::println(" [{}None{}]",Color::DimGray,Color::Clear);
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

		std::string techdisplay = (tech == nullptr) ? std::format("1 - Technique [{}None{}] ",Color::DimGray, Color::Clear) : std::format("1 - Technique [{}] ", tech->GetTechniqueName());
		std::string fightdisplay = "2 - Fight";
		std::string specdisplay = (special == nullptr) ? std::format("3 - Special [{}None{}] ",Color::DimGray, Color::Clear) : std::format("3 - Special [{}] ", special->GetSpecialSimplifiedName());
		std::string domaindisplay = (domain == nullptr && counter == nullptr) ? std::format("4 - Domain [{}Locked{}]",Color::DimGray, Color::Clear) : "4 - Domain Actions";
		std::string tauntdisplay = "5 - Taunt";
		std::string rctdisplay = s->IsHeavenlyRestricted() ? std::format("6 - Reverse Cursed Technique [{}Locked{}]",Color::DimGray, Color::Clear) : std::format("6 - Reverse Cursed Technique [{}]", s->GetRCTstatus());
		std::string ampdisplay = s->IsHeavenlyRestricted() ? std::format("7 - Domain Amplification [{}Locked{}]",Color::DimGray, Color::Clear) : std::format("7 - Domain Amplification [{}]",s->GetDAstatus());
		std::string tooldisplay = (s->GetCursedTools().empty() && s->GetTool() == nullptr) ? std::format("8 - Cursed Tools [{}None{}]",Color::DimGray, Color::Clear) : "8 - Cursed Tools";
		std::string setdisplay = (tech == nullptr) ? std::format("9 - Technique Settings [{}Locked{}]",Color::DimGray, Color::Clear) : "9 - Technique Settings";
		std::string shikigamidisplay = (s->GetShikigami().empty()) ? std::format("10 - Shikigami [{}None{}]",Color::DimGray, Color::Clear) : "10 - Shikigami";

		int col_width = 35;
		std::println("  {} | {} | {}",
			ANSIPadding(techdisplay, col_width),
			ANSIPadding(fightdisplay, col_width),
			ANSIPadding(specdisplay, col_width));

		std::println("  {} | {} | {}",
			ANSIPadding(domaindisplay, col_width),
			ANSIPadding(tauntdisplay, col_width),
			ANSIPadding(rctdisplay, col_width));

		std::println("  {} | {} | {}",
			ANSIPadding(ampdisplay, col_width),
			ANSIPadding(tooldisplay, col_width),
			ANSIPadding(setdisplay, col_width));

		std::println("  {} |", ANSIPadding(shikigamidisplay, col_width));
		

		std::print("\n=> ");
	}
}
void UserInterface::ContinuePrompt() {
	std::println("Press Enter to continue...");
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}

void UserInterface::ClearScreen() {
	std::print("\033[2J\033[H");
}