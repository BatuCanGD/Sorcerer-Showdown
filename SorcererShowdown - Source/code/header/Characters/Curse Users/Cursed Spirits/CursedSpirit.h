#pragma once
#include "CurseUser.h"
#include <string>

class CursedSpirit : public CurseUser {
protected:
	double passive_health_regen = 5.0;
public:
	CursedSpirit(double hp, double ce, double regen);
	std::string GetSimpleName() const override;
	std::string GetName() const override;
	void Attack(Character*) override;
	bool IsaCursedSpirit() const override;
	bool CanBeHit() const override;
};