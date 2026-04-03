#pragma once
#include "Sorcerer.h"

class Gojo : public Sorcerer {
public:
	Gojo();
	std::unique_ptr<Character> Clone() const override;
	std::string GetSimpleName() const override;
	std::string GetName() const override;
	void OnCharacterTurn(Character*, std::vector<std::unique_ptr<Character>>&) override;
	bool CanBeHit() const override;
};