#pragma once
#include "Shikigami.h"

class Agito : public Shikigami {
public:
	Agito();
protected:
	const double passive_heal_amount = 35.0;
	const double summon_amount = 150.0;
public:
	std::unique_ptr<Character> Clone() const override;

	void PassiveSupport(Character* user);
	void OnCharacterTurn(Character*, std::vector<std::unique_ptr<Character>>&) override;

	std::string GetSimpleName() const override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};