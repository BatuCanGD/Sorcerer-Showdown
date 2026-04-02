#pragma once
#include <vector>
#include <memory>

class Sorcerer;

class PlayerManager {
public:
	void OnPlayerTurn(Sorcerer& s, const std::vector<std::unique_ptr<Sorcerer>>& battlefield);
	void PlayerRCTusage(Sorcerer& s);
	void PlayerDAusage(Sorcerer& s);
	void PlayerDomainUsage(Sorcerer& s);
	void PlayerShikigami(Sorcerer& s);
	void PlayerReinforcement(Sorcerer& s);
	void GetPlayerTools(Sorcerer& s);
	Sorcerer* TargetSelector(const std::vector<std::unique_ptr<Sorcerer>>& battlefield, Sorcerer* player);

};