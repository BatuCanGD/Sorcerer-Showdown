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
	bool IsActivePhysically() const;
	virtual void OnShikigamiTurn(Sorcerer*) = 0;		
	std::string GetShikigamiStatus() const;
	std::string GetName() const override;
	bool CanBeHit() const override;
};

class Mahoraga : public Shikigami {
public:
	Mahoraga();
protected:
	const double keep_active_cost = 200.0;
	enum class InfinityAdaptation {
		None,
		FirstSpin,
		SecondSpin,
		ThirdSpin,
		FourthSpin
	};
	InfinityAdaptation InfStage = InfinityAdaptation::None;
public:
	void Adapt();
	bool FullyAdapted()const;
	void OnShikigamiTurn(Sorcerer*) override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};

class Agito : public Shikigami {
public:
	Agito();
protected:
	const double passive_heal_amount = 35.0;
	const double summon_amount = 150.0;
public:
	void PassiveSupport(Sorcerer* user);
	void OnShikigamiTurn(Sorcerer*) override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};

class Rika : public Shikigami {
private:
	bool value_saved = false;
	double user_ce;
	double user_regen;
	static constexpr double rika_ce_increase = 100000.0;
	static constexpr double rika_regen_increase = 10000.0;
	static constexpr int max_active_time = 6;
	int active_cooldown = 5;
public:
	Rika();
	void OnShikigamiTurn(Sorcerer*) override;
	void SaveUserCursedEnergy(Sorcerer*);
	void RikaCooldownRegeneration(Sorcerer*);
	std::string GetName() const override;
	bool CanBeHit() const override;
};