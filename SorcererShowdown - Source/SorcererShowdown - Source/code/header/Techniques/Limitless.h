#pragma once
#include "Techniques.h"

class Limitless : public Technique { 
protected:
	bool Infinity = true;
	static constexpr double blue_output = 20.0 + base_output;
	static constexpr double red_output = 40.0 + base_output;
	static constexpr double purple_output = 90.0 + base_output;
public:
	std::unique_ptr<Technique> Clone() const override;

	enum class LimitlessType { Blue = 1, Red = 2, Purple = 3 };

	void SetInfinity(bool s);
	void InfinityNerf(Sorcerer*); // drain CE
	bool CheckInfinity() const;

	void BlueTechniqueDamageTarget(Sorcerer* user, Character* target);
	void RedTechniqueDamageTarget(Sorcerer* user, Character* target);
	void PurpleTechniqueDamageTarget(Sorcerer* user, Character* target);
	void UseTheLimitlessTechnique(LimitlessType choice, Sorcerer* s, Character* c);
	std::string GetTechniqueName() const override;
	void TechniqueMenu(Sorcerer* user, Character* target) override;
	void TechniqueSetting(Sorcerer*, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) override;
	void Chant() override;
};
