#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

class Sorcerer;
class Character;

class BattleManager {
public:
	bool GameEndCheck(const std::vector<std::unique_ptr<Sorcerer>>& battlefield, bool spectator_mode);
	bool SetupBattlefield(std::vector<std::unique_ptr<Sorcerer>>& battlefield, std::map<std::string, int>& sorcerer_counts);
	bool SkipTurnFullyCheck();
	bool ManageEndOfTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield, bool spectator_mode);
	void DomainCheckAndPerform(std::vector<std::unique_ptr<Sorcerer>>& battlefield);
	bool IsBattleOver(bool,bool,bool, std::vector<std::unique_ptr<Sorcerer>>&);
};