#include "Character.h"
#include "Sorcerer.h"
#include "CombatContext.h"

import std;
using namespace std;

int main() { // main
	vector<unique_ptr<Sorcerer>> battlefield;
	battlefield.push_back(make_unique<Sukuna>()); 
	battlefield.push_back(make_unique<Gojo>()); 
	battlefield.push_back(make_unique<Sukuna>()); 
	Sorcerer* player = battlefield[0].get();

	CombatContext combat;

	if (battlefield.size() < 2) {
		println("Not enough sorcerers to start the fight");
		return 1;
	}

	println("-------The battle between {} sorcerers begin!-------", battlefield.size());
	println("-----------------------------------------------------");

	
	while (true) {

		for (const auto& s : battlefield) {
			if (player == s.get()) {
				println("-------Player's ({}'s) Turn-------", player->GetName());
				println("player is a fraud");
				for (int i = 0; i < 2; i++) println(" ");
			}
			else {
				println("-------{}'s Turn-------", s->GetName());
				s->OnSorcererTurn();
				for (int i = 0; i < 2; i++) println(" ");
			}
		}

		println("press enter to Continue...");
		cin.ignore();

		if (battlefield[0]->GetCharacterHealth() <= 0.0) {
			println("You have been defeated! Game Over.");
			break;
		}
		else if (battlefield.size() <= 1) {
			println("You have defeated all the sorcerers! Victory!");
			break;
		}
		for (int i = 0; i < 20; i++) println(" ");
	}
	println("press enter to end the game...");
	cin.ignore();
	return 0;
}
