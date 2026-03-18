#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

class Sorcerer;
class Character;
class CombatContext;

class BattleManager {
public:
	bool GameEndCheck(const std::vector<std::unique_ptr<Sorcerer>>& battlefield);
	void ShowBattleEntry(const std::vector<std::unique_ptr<Sorcerer>>& battlefield);
	void OnPlayerTurn(Sorcerer& s, const std::vector<std::unique_ptr<Sorcerer>>& battlefield, CombatContext& context);
	void PlayerRCTusage(Sorcerer& s);
	void PlayerDAusage(Sorcerer& s);
	void GetPlayerTools(Sorcerer& s);
	void SetupBattlefield(std::vector<std::unique_ptr<Sorcerer>>& battlefield, std::map<std::string, int>& sorcerer_counts);
	void DisplaySorcererStatus(Sorcerer* s);
	bool ManageEndOfTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield);
	void DomainCheckAndPerform(std::vector<std::unique_ptr<Sorcerer>>& battlefield);
	Sorcerer* TargetSelector(const std::vector<std::unique_ptr<Sorcerer>>& battlefield, Sorcerer* player);
	void ClearScreen();
};