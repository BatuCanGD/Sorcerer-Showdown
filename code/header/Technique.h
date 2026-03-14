#pragma once

#include <string>

class Sorcerer;
class Character;

class Technique { // BASE CLASS
public:
	enum class Status { Usable, DomainBoost, BurntOut };
protected:
	static constexpr double base_output = 10.0;
	Status state = Status::Usable;
public:
	virtual ~Technique() = default;
	void Set(Status s);
	Status GetStatus() const;
	double GetTechniqueOutput() const;
	double CalculateDamage(Sorcerer* user, double cost);
	std::string GetStringStatus() const;
	virtual std::string GetTechniqueName() const = 0;
};

class Limitless : public Technique { // LIMITLESS
protected:
	bool Infinity = true;
	static constexpr double blue_output = 20.0 + base_output, red_output = 40.0 + base_output, purple_output = 90.0 + base_output;
	enum class TechniqueChants { 
		None,
		FirstChant,
		SecondChant,
		ThirdChant,
		FourthChant
	};
public:
	enum class LimitlessType { Blue = 1, Red = 2, Purple = 3 };

	void SetInfinity(bool s);
	void InfinityNerf(); // drain CE
	bool CheckInfinity() const;

	double BlueTechniqueDamageTarget(Sorcerer* user, Character* target);
	double RedTechniqueDamageTarget(Sorcerer* user, Character* target);
	double PurpleTechniqueDamageTarget(Sorcerer* user, Character* target);
	void UseTheLimitlessTechnique(LimitlessType choice, Sorcerer* s, Character* c);
	std::string GetTechniqueName() const override;
};


class Shrine : public Technique { // SHRINE
protected:
	static constexpr double slash_output = 45.0, cleave_output = 300.0, wcs_output = 1000.0;
	bool world_cutting_slash_allowed = false;
public:
	enum class ShrineType { Dismantle = 1, Cleave = 2 };

	void SetWCS(bool s);
	std::string GetTechniqueName() const override;

	double CleaveTechniqueDamageTarget(Sorcerer* user, Character* target);
	double DismantleTechniqueDamageTarget(Sorcerer* user, Character* target);
	void UseShrineTechnique(ShrineType choice, Sorcerer* s, Character* c);
};

