#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

class Character;
struct Battlefield;

class BattleManager {
public:
	bool GameEndCheck(const std::vector<std::unique_ptr<Character>>& battlefield, bool spectator_mode);
	bool SetupBattlefield(std::vector<std::unique_ptr<Character>>& battlefield, std::map<std::string, int>& sorcerer_counts);
	bool SkipTurnFullyCheck();
	void SpawnNewFighters(Battlefield&);
	bool ManageEndOfTurn(std::vector<std::unique_ptr<Character>>& battlefield, bool spectator_mode);
	void DomainCheckAndPerform(std::vector<std::unique_ptr<Character>>& battlefield);
	bool IsBattleOver(bool,bool,bool, std::vector<std::unique_ptr<Character>>&);
};