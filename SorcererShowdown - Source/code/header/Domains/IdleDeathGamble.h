#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class IdleDeathGamble : public Domain {
protected:
	static constexpr double domain_cost = 25.0;
	int luck = 200;
	bool jackpot = false, info_dumped = false;
public:
	IdleDeathGamble();
	bool HasHitJackpot() const;
	void SetJackpot(bool);
	void OnSureHit(CurseUser& user, Character& target) override;
	double GetUseCost() const override;
};