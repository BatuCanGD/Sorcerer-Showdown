#pragma once
#include "Domain.h"

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