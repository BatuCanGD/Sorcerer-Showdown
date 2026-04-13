#pragma once
#include "Sorcerer.h"

class Yuta : public Sorcerer {
public:
	Yuta();
	std::unique_ptr<Character> Clone() const override;
	std::string GetSimpleName() const override;
	std::string GetName() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
	bool InfCheck(Character*);
	void HitCharacter(Character*);
	bool CanBeHit() const override;
};