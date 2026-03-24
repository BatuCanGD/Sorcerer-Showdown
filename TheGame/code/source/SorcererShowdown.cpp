#include "Sorcerer.h"
#include "Specials.h"
#include "BattleManager.h"

import std;

int main() { // main
	BattleManager manager;

	std::vector<std::unique_ptr<Sorcerer>> battlefield;
	std::map<std::string, int> sorcerer_counts;

	manager.SetupBattlefield(battlefield, sorcerer_counts);
	manager.ShowBattleEntry(battlefield);
	
	battlefield[0]->SetAsPlayer(true);

	while (true) {
		bool game_over = false;
		for (const auto& s : battlefield) {
			if (s->GetCharacterHealth() <= 0.0) continue;

			if (s->IsThePlayer()) {
				manager.DisplaySorcererStatus(s.get());
				if (s->IsCharacterStunned()) continue;
				std::println("\n");
				manager.OnPlayerTurn(*s, battlefield);
				std::println("\n");
				std::cin.clear();
			}
			else {
				std::println("\n");
				manager.DisplaySorcererStatus(s.get());
				std::println("\n");
				s->OnSorcererTurn(battlefield);
				std::println("\n");
			}

			if (manager.GameEndCheck(battlefield) == true) {
				game_over = true;
				break;
			}
			std::println("Press Enter to continue to the next sorcerers turn...");
			
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin.get();
		}
		manager.DomainCheckAndPerform(battlefield);
		bool player_found = manager.ManageEndOfTurn(battlefield);

		if (manager.IsBattleOver(game_over, player_found, battlefield)) break;

		std::println("Press Enter to begin the next round...");
		std::cin.get();

		manager.ClearScreen();
	}
	std::println("press enter to end the game...");
	std::cin.get();
	return 0;
}