#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class MalevolentShrine : public Domain {
protected:
	static constexpr double domain_cost = 750.0;
	static constexpr double surehit_slashdamage = 100.0;
public:
	MalevolentShrine();
	std::unique_ptr<Domain> Clone() const override;
	void OnSureHit(CurseUser& user, Character& target) override;
	double GetUseCost() const override;
};