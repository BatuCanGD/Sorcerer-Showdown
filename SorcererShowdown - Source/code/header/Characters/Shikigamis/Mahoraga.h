#pragma once
#include "Shikigami.h"

class Mahoraga : public Shikigami {
public:
	Mahoraga();
protected:
	const double keep_active_cost = 120.0;
	enum class InfinityAdaptation {
		None,
		FirstSpin,
		SecondSpin,
		ThirdSpin,
		FourthSpin
	};
	InfinityAdaptation InfStage = InfinityAdaptation::None;
	InfinityAdaptation PrevState = InfStage;
public:
	std::unique_ptr<Character> Clone() const override;

	void Adapt();
	bool FullyAdapted()const;
	void OnCharacterTurn(Character*, Battlefield&) override;
	void PrintStatus(Character*) const;
	void UpdatePreviousState();

	bool CanBeHit() const override;
};