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
	void PartiallyManifest();
	void Manifest();
	void Withdraw();
	void ActiveTimeIncrementor();
	int GetActiveTime() const;
	bool IsActive() const;
	bool IsPartiallyActive() const;
	bool IsActivePhysically() const;
	virtual void OnShikigamiTurn(Sorcerer*) = 0;		
	std::string GetShikigamiStatus() const;

	std::string GetSimpleName() const override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};