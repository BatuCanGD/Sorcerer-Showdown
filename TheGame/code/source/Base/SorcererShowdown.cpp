#include "Sorcerer.h"
#include "Specials.h"
#include "BattleManager.h"
#include "PlayerManager.h"
#include "UIDisplay.h"

import std;

int main() { // main
	BattleManager manager;
	PlayerManager player;
	UserInterface interface;

	std::vector<std::unique_ptr<Sorcerer>> battlefield;
	std::map<std::string, int> sorcerer_counts;

	bool spectator_mode = manager.SetupBattlefield(battlefield, sorcerer_counts);
	bool skip_turns = manager.SkipTurnFullyCheck();
	interface.ShowBattleEntry(battlefield);
	
	if (!spectator_mode) {
		battlefield[0]->SetAsPlayer(true);
	}
	while (true) {
		bool game_over = false;
		for (const auto& s : battlefield) {
			if (s->GetCharacterHealth() <= 0.0) continue;
			if (s->IsThePlayer()) {
				interface.DisplaySorcererStatus(s.get());
				if (s->IsCharacterStunned()) continue;
				std::println("\n");
				player.OnPlayerTurn(*s, battlefield);
				std::println("\n");
				std::cin.clear();
			}
			else {
				std::println("\n");
				interface.DisplaySorcererStatus(s.get());
				std::println("\n");
				s->OnSorcererTurn(battlefield);
				std::println("\n");
			}

			if (manager.GameEndCheck(battlefield, spectator_mode)) {
				game_over = true;
				break;
			}
			if (skip_turns) {
				std::println("Press Enter to continue to the next sorcerers turn...");
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cin.get();
			}
		}
		manager.DomainCheckAndPerform(battlefield);
		bool player_found = manager.ManageEndOfTurn(battlefield, spectator_mode);

		if (manager.IsBattleOver(game_over, player_found, spectator_mode ,battlefield)) break;

		std::println("Press Enter to begin the next round...");
		std::cin.get();
		interface.ClearScreen();
	}
	std::println("press enter to end the game...");
	std::cin.get();
	return 0;
}