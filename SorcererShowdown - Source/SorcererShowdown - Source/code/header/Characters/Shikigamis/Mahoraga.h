#pragma once
#include "Shikigami.h"

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