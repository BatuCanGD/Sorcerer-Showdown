#pragma once
#include "Domain.h"

class HollowWickerBasket : public Domain {
private:
	static constexpr double domain_cost = 100.0;
public:
	HollowWickerBasket();
	void OnSureHit(Character&) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};