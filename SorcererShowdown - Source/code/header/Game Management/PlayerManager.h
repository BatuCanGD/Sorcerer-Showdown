#pragma once
#include <vector>
#include <memory>

class Sorcerer;
class Character;

class PlayerManager {
public:
	void OnPlayerTurn(Character& s, const std::vector<std::unique_ptr<Character>>& battlefield);
	void PlayerRCTusage(Character& s);
	void PlayerDAusage(Character& s);
	void PlayerDomainUsage(Character& s);
	void PlayerShikigami(Character& s);
	void PlayerReinforcement(Character& s);
	void GetPlayerTools(Character& s);
	Character* TargetSelector(const std::vector<std::unique_ptr<Character>>& battlefield, Character* player);

};