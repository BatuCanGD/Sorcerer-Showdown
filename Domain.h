#pragma once

class Character;
class Sorcerer;

class Domain {
protected:
	double domain_health = 200.0;
	const double base_range = 15;
	double current_range = 15;
	bool clashing = false;
public:
	virtual ~Domain() = default;
	bool Clashing() const;
	void SetClashState(bool a);
	double DomainRangeMult()const;
	virtual void OnSureHit(Character& target) = 0;
};


class InfiniteVoid : public Domain {
protected:
	static constexpr double surehit_braindamage = 30.0;
public:
	void OnSureHit(Character& target) override;
};
class MalevolentShrine : public Domain {
protected:
	static constexpr double surehit_slashdamage = 75.0;
public:
	void OnSureHit(Character& target) override;
};