#pragma once
#include "Shikigami.h"

class Agito : public Shikigami {
public:
	Agito();
protected:
	const double passive_heal_amount = 50.0;
	const double summon_amount = 80.0;
public:
	void PassiveSupport(Character* user);
	void OnCharacterTurn(Character*, Battlefield&) override;
};