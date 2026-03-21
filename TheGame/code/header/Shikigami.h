#pragma once
#include <string>
#include "Character.h"
#include "Sorcerer.h"

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
	bool IsActive() const;
	bool IsActivePhysically() const;
	virtual void OnShikigamiTurn(Sorcerer*) = 0;		
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
public:
	Rika();
	void OnShikigamiTurn(Sorcerer*) override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};