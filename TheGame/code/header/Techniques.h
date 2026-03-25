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

class Limitless : public Technique { // LIMITLESS
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


class Shrine : public Technique { // SHRINE
protected:
	static constexpr double slash_output = 45.0;
	static constexpr double cleave_output = 300.0;
	static constexpr double wcs_output = 2500.0;
	bool world_cutting_slash_allowed = false;
public:
	std::unique_ptr<Technique> Clone() const override;

	enum class ShrineType { Dismantle = 1, Cleave = 2, WCS = 3 };

	void SetWCS(bool s);
	bool WorldCuttingSlashUnlocked() const;
	std::string GetTechniqueName() const override;

	void CleaveTechniqueDamageTarget(Sorcerer* user, Character* target);
	void DismantleTechniqueDamageTarget(Sorcerer* user, Character* target);
	void WorldCuttingSlashToTarget(Sorcerer* user, Character* target);
	void UseShrineTechnique(ShrineType choice, Sorcerer* s, Character* c);
	void TechniqueMenu(Sorcerer* user, Character* target) override;
	void TechniqueSetting(Sorcerer*, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) override;
	void Chant() override;
};

class Copy : public Technique {
private:
	std::vector<std::unique_ptr<Technique>> copied_techniques;
	static constexpr int max_copies = 5;
	int active_copy = -1;
public:
	std::unique_ptr<Technique> Clone() const override;

	void CopyFrom(Sorcerer* target);
	void SwitchCopy(int index);              
	Technique* GetActive() const;             
	void Set(Status s) override;

	void TechniqueMenu(Sorcerer* user, Character* target) override;
	void TechniqueSetting(Sorcerer* user, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) override;
	void Chant() override;
	std::string GetTechniqueName() const override;
};

