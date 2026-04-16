#pragma once
#include <string>
#include <vector>
#include <memory>

class CurseUser;
class Character;
struct Battlefield;

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
	bool FullyChanted()const;
	double GetTechniqueOutput() const;
	double CalculateDamage(CurseUser* user, double cost) const;
	std::string GetStringStatus() const;

	virtual std::string GetTechniqueName() const = 0;
	virtual std::string GetTechniqueSimpleName() const = 0;

	virtual void Chant() = 0;
	virtual void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) = 0;
	virtual void TechniqueSetting(CurseUser*, Battlefield&) = 0;
	virtual std::unique_ptr<Technique> Clone() const = 0;

	std::string GetStringChantLevel() const;
	double GetChantPower() const;

	bool Boosted() const;
	bool BurntOut() const;
	bool Usable() const;
};

