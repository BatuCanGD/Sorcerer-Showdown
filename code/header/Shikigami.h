#pragma once
#include <string>
#include "Character.h"
#include "Sorcerer.h"

class Shikigami : public Character {
protected:
	int active_turn_amount = 0;
	enum class ShikigamiStatus {
		Shadow,
		PartialManifestation,
		Manifested
	};
	ShikigamiStatus shikigami_stats = ShikigamiStatus::Shadow;
public:
	Shikigami(double hp, double ce, double re);
	void PartiallyManifest();
	void Manifest();
	void Withdraw();
	void ActiveTimeIncrementor();
	bool IsActive() const;
	virtual void OnShikigamiTurn() = 0;		
	std::string GetName() const override;
};

class Mahoraga : public Shikigami {
public:
	Mahoraga();
protected:
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
	bool FullyAdaptedToInfinity()const;
	void OnShikigamiTurn() override;
	std::string GetName() const override;
};

class Agito : public Shikigami {
public:
	Agito();
protected:
	const double passive_heal_amount = 10.0;
	const double summon_amount = 20.0;
public:
	void PassiveSupport(Sorcerer* user);
	void OnShikigamiTurn() override;
	std::string GetName() const override;
};