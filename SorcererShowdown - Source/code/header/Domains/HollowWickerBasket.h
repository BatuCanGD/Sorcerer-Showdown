#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class HollowWickerBasket : public Domain {
private:
	static constexpr double domain_cost = 100.0;
public:
	HollowWickerBasket();
	void OnSureHit(CurseUser& user, Character&) override;
	double GetUseCost() const override;
};