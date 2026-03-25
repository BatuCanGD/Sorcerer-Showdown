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


class InfiniteVoid : public Domain {
protected:
	static constexpr double domain_cost = 1000.0;
	static constexpr double surehit_braindamage = 30.0;
public:
	InfiniteVoid();
	void OnSureHit(Character& target) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};

class MalevolentShrine : public Domain {
protected:
	static constexpr double domain_cost = 750.0;
	static constexpr double surehit_slashdamage = 75.0;
public:
	MalevolentShrine();
	void OnSureHit(Character& target) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};

class AuthenticMutualLove : public Domain {
protected:
	static constexpr double domain_cost = 500.0;
	static constexpr double imbued_surehit_damage = 60.0;
public:
	AuthenticMutualLove();
	void OnSureHit(Character& target) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};

// neutralizer domains

class SimpleDomain : public Domain {
private:
	static constexpr double domain_cost = 0.0;
public:
	SimpleDomain();
	void OnSureHit(Character&) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};
class HollowWickerBasket : public Domain {
private:
	static constexpr double domain_cost = 0.0;
public:
	HollowWickerBasket();
	void OnSureHit(Character&) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};

// fun domain

class KillEveryoneDomain : public Domain {
protected:
	static constexpr double surehit_killdamage = INT32_MAX;
	static constexpr double domain_cost = 5000000.0;
public:
	KillEveryoneDomain();
	void OnSureHit(Character& target) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};