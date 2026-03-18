#include "Sorcerer.h"
#include "CombatContext.h"
#include "BattleManager.h"
import std;

int main() { // main
	CombatContext context;
	BattleManager manager;

	std::vector<std::unique_ptr<Sorcerer>> battlefield;
	std::map<std::string, int> sorcerer_counts;

	manager.SetupBattlefield(battlefield, sorcerer_counts);
	manager.ShowBattleEntry(battlefield);
	
	battlefield[0]->SetAsPlayer(true); // set the first sorcerer inserted as the player

	while (true) {
		bool game_over = false;
		for (const auto& s : battlefield) {
			if (s->GetCharacterHealth() <= 0.0) continue;

			if (s->IsThePlayer()) {
				manager.DisplaySorcererStatus(s.get());
				manager.OnPlayerTurn(*s, battlefield, context);
				std::println("\n");
				std::cin.clear();
			}
			else {
				manager.DisplaySorcererStatus(s.get());
				s->OnSorcererTurn(battlefield);
				std::println("\n");
			}

			if (manager.GameEndCheck(battlefield) == true) {
				game_over = true;
				break;
			}
			std::println("Press Enter to continue to the next turn...");
			
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin.get();
		}

		manager.DomainCheckAndPerform(battlefield);
		bool player_found = manager.ManageEndOfTurn(battlefield);

		std::cin.get();
		if (!player_found) {
			std::println("\nYou have been defeated! Game Over.");
			break;
		}
		else if (battlefield.size() == 1 || game_over) {
			std::println("\nCongratulations! You have defeated all other sorcerers and won the battle!");
			break;
		}
		manager.ClearScreen();
	}
	std::println("press enter to end the game...");
	std::cin.get();
	return 0;
}