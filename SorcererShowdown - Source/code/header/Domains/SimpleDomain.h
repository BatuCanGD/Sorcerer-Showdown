#pragma once
#include "Domain.h"

class SimpleDomain : public Domain {
private:
	static constexpr double domain_cost = 0.0;
public:
	SimpleDomain();
	void OnSureHit(Character&) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};