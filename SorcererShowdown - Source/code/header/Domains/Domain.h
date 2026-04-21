#pragma once
#include <string>

class Character;
class CurseUser;

class Domain {
protected:
	double domain_health;
	const double base_health;
	double domain_overwhelm_strength;
	const double base_range;
	double current_range;
	bool clashing = false;
	bool is_neutralizer = false;

	enum class Refinement {
		Unstable, Crude, Refined, Absolute
	};
	Refinement ref_level = Refinement::Refined;
	enum class HitType {
		HitsEveryone, HitsCurseUsers
	};
	HitType hit_type = HitType::HitsCurseUsers;
public:
	Domain(double health, double damage, double range);
	virtual ~Domain() = default;
	bool Clashing() const;
	void SetClashState(bool a);
	double DomainRangeMult()const;
	virtual void OnSureHit(CurseUser& user, Character& target) = 0;
	virtual std::string GetDomainName() const = 0;
	virtual double GetUseCost() const = 0;

	double GetDomainHealth() const;
	double GetDomainStrength() const;
	double GetDomainRange() const;

	bool CheckDomainSurehit(Character&) const;

	void DamageDomain(double);
	void ClashDomains(CurseUser&, CurseUser&);
	bool GetRefinementComparison(Domain&, Domain&) const;
	Refinement GetRefinement() const;
	HitType GetHitType() const;
	void CollapseDomain();

	bool IsDestroyed() const;
	bool IsNeutralizer() const;
};