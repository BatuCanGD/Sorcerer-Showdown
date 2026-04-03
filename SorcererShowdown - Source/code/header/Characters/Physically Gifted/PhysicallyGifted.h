#pragma once

#include "Character.h"

class PhysicallyGifted : public Character {
protected:
	double strength;
	double strength_to_reinforcement_ratio = 500.0;
	double strength_to_damage_ratio = 35.0;
public:
	PhysicallyGifted(double hp, double str);
	virtual ~PhysicallyGifted() = default;
	std::unique_ptr<Character> Clone() const override;

	double GetStrength() const;
	double GetStrengthDamage()const;
	double GetDamageReinforcement()const override;
	bool IsPhysicallyGifted() const override;

	std::string GetSimpleName() const override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};