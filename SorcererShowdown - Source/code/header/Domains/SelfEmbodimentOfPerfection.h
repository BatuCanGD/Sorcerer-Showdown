#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class SelfEmbodimentOfPerfection : public Domain {
protected:
	static constexpr double domain_cost = 500.0;
	static constexpr double surehit_damage = 150.0;
public:
	SelfEmbodimentOfPerfection();
	void OnSureHit(CurseUser& user, Character& target) override;
	double GetUseCost() const override;
};