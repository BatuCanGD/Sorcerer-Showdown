#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class AuthenticMutualLove : public Domain {
protected:
	static constexpr double domain_cost = 500.0;
	static constexpr double imbued_surehit_damage = 125.0;
public:
	AuthenticMutualLove();
	void OnSureHit(CurseUser& user, Character& target) override;
	double GetUseCost() const override;
};