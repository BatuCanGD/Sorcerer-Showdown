#pragma once

#include <string>
#include <vector>
#include <memory>

class Sorcerer;
class Character;

class Technique { // BASE CLASS
protected:
	static constexpr double base_output = 10.0;
	enum class ChantLevel {
		Zero, One, Two, 
		Three, Four
	};
	ChantLevel chant = ChantLevel::Zero;
public:
	enum class Status { Usable, DomainBoost, BurntOut };
	Status state = Status::Usable;

	virtual ~Technique() = default;
	virtual void Set(Status s);
	Status GetStatus() const;
	double GetTechniqueOutput() const;
	double CalculateDamage(Sorcerer* user, double cost) const;
	std::string GetStringStatus() const;
	virtual std::string GetTechniqueName() const = 0;
	virtual void Chant() = 0;
	virtual void TechniqueMenu(Sorcerer* user, Character* target) = 0;
	virtual void TechniqueSetting(Sorcerer*, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) = 0;
	virtual std::unique_ptr<Technique> Clone() const = 0;

	std::string GetStringChantLevel() const;
	double GetChantPower() const;

	bool Boosted() const;
	bool BurntOut() const;
	bool Usable() const;
};

