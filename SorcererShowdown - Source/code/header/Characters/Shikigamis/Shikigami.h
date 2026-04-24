#pragma once
#include "Character.h"
#include <string>

class Sorcerer;

class Shikigami : public Character {
protected:
	static constexpr double shadow_health_regen = 30.0;
	int active_turn_amount = 0;
	enum class State {
		Shadow,
		Partial,
		Full
	};
	State shikigami_state = State::Shadow;
public:
	Shikigami(double hp, double ce, double re);
	bool CanBeAssignedID() const override;

	void PartiallyManifest();
	void Manifest();
	void Withdraw();
	void ActiveTimeIncrementor();
	int GetActiveTime() const;
	bool IsActive() const;
	bool IsPartiallyActive() const;
	bool IsActivePhysically() const;	
	std::string GetShikigamiStatus() const;

	bool IsShikigami() const override;
	bool CanBeHit() const override;
};