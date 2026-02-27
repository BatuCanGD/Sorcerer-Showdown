#pragma once

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
};

class Limitless : public Technique { // LIMITLESS
protected:
	static constexpr double blue_output = 20.0 + base_output, red_output = 40.0 + base_output, purple_output = 90.0 + base_output;
	enum class TechniqueChants {
		None,
		FirstChant,
		SecondChant,
		ThirdChant,
		FourthChant
	};
public:
	double CalculateDamage(Sorcerer* user, double cost);
	double BlueTechniqueDamageTarget(Sorcerer* user, Character* target);
	double RedTechniqueDamageTarget(Sorcerer* user, Character* target);
	double PurpleTechniqueDamageTarget(Sorcerer* user, Character* target);
	double GetTechniqueOutput() const;
	void UseTheLimitlessTechnique(int choice, Sorcerer* s, Character* c);
};


class Shrine : public Technique { // SHRINE
protected:
	static constexpr double slash_output = 75.0, cleave_output = 1000.0, wcs_output = 350.0;
	bool world_cutting_slash_allowed = false;
public:
	void SetWCS(bool s);
};