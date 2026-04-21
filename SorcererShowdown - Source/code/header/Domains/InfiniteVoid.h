#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class InfiniteVoid : public Domain {
protected:
	static constexpr double domain_cost = 1000.0;
	static constexpr double surehit_braindamage = 100.0;
public:
	InfiniteVoid();
	void OnSureHit(CurseUser& user, Character& target) override;
	std::string GetDomainName() const override;
	double GetUseCost() const override;
};