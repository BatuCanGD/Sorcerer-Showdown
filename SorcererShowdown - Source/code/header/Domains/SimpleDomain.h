#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class SimpleDomain : public Domain {
private:
	static constexpr double domain_cost = 150.0;
public:
	SimpleDomain();
	void OnSureHit(CurseUser& user, Character&) override;
	double GetUseCost() const override;
};