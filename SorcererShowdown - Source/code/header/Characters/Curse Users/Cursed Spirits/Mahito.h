#pragma once
#include "CursedSpirit.h"
#include <vector>
#include <string>

class Mahito : public CursedSpirit {
public:
	Mahito();
	std::unique_ptr<Character> Clone() const override;
	std::string GetSimpleName() const override;
	std::string GetName() const override;
	bool CanBeHit() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
	void TickCharacterSpecialty() override;
};