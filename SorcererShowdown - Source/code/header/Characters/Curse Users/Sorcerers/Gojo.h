#pragma once
#include "Sorcerer.h"

class Gojo : public Sorcerer {
public:
	Gojo();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
	bool CanBeHit() const override;
	bool InfCheck(Character*);
	void HitCharacter(Character*);
};