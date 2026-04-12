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
	void OnCharacterTurn(Character*, std::vector<std::unique_ptr<Character>>&) override;
	void TickCharacterSpecialty() override;
};