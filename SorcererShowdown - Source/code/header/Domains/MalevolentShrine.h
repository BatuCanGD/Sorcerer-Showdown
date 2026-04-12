#pragma once
#include "Domain.h"

class MalevolentShrine : public Domain {
protected:
	static constexpr double domain_cost = 750.0;
	static constexpr double surehit_slashdamage = 150.0;
public:
	MalevolentShrine();
	void OnSureHit(Character& target) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};