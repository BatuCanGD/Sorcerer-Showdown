#pragma once
#include <string>

class Character;
class Sorcerer;

class Domain {
protected:
	double domain_health;
	const double base_health;
	double domain_overwhelm_strength;
	const double base_range;
	double current_range;
	bool clashing = false;
	bool is_neutralizer = false;
public:
	Domain(double, double, double);
	virtual ~Domain() = default;
	bool Clashing() const;
	void SetClashState(bool a);
	double DomainRangeMult()const;
	virtual void OnSureHit(Character& target) = 0;
	virtual std::string GetDomainName() const = 0;
	virtual double GetUseCost() const = 0;

	double GetDomainHealth() const;
	double GetDomainStrength() const;
	double GetDomainRange() const;

	void DamageDomain(double);
	void ClashDomains(Sorcerer&,Sorcerer&);
	void CollapseDomain();

	bool IsDestroyed() const;
	bool IsNeutralizer() const;
};