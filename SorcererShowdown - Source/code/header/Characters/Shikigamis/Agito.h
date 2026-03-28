#pragma once
#include "Shikigami.h"

class Agito : public Shikigami {
public:
	Agito();
protected:
	const double passive_heal_amount = 35.0;
	const double summon_amount = 150.0;
public:
	void PassiveSupport(Sorcerer* user);
	void OnShikigamiTurn(Sorcerer*) override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};