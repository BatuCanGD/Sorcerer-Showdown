#pragma once
#include <string>


class Character;
class Sorcerer;

class Domain {
protected:
	double domain_health;
	double domain_overwhelm_strength;
	const double base_range;
	double current_range;
	bool clashing = false;
public:
	Domain(double, double, double);
	virtual ~Domain() = default;
	bool Clashing() const;
	void SetClashState(bool a);
	double DomainRangeMult()const;
	virtual void OnSureHit(Character& target) = 0;
	virtual std::string GetDomainName() const = 0;

	double GetDomainHealth() const;
	double GetDomainStrength() const;
	double GetDomainRange() const;
};


class InfiniteVoid : public Domain {
protected:
	static constexpr double surehit_braindamage = 30.0;
public:
	InfiniteVoid();
	void OnSureHit(Character& target) override;
	std::string GetDomainName() const override;
};

class MalevolentShrine : public Domain {
protected:
	static constexpr double surehit_slashdamage = 75.0;
public:
	MalevolentShrine();
	void OnSureHit(Character& target) override;
	std::string GetDomainName() const override;
};

class KillEveryoneDomain : public Domain {
protected:
	static constexpr double surehit_killdamage = INT32_MAX;
public:
	KillEveryoneDomain();
	void OnSureHit(Character& target) override;
	std::string GetDomainName() const override;
};