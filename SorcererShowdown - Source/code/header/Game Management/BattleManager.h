#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

class Character;
struct Battlefield;

class BattleManager {
public:
	bool GameEndCheck(Battlefield&, bool spectator_mode);
	bool SetupBattlefield(Battlefield&);
	bool SkipTurnFullyCheck();
	void SpawnNewFighters(Battlefield&);
	bool ManageEndOfTurn(Battlefield&, bool spectator_mode);
	void DomainCheckAndPerform(Battlefield&);
	bool IsBattleOver(bool,bool,bool, Battlefield&);
};