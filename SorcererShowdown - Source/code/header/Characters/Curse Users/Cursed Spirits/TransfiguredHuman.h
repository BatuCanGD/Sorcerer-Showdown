#pragma once
#include "CursedSpirit.h"
#include <vector>
#include <string>

class TransfiguredHuman : public CursedSpirit {
public:
	TransfiguredHuman();
	std::unique_ptr<Character> Clone() const override;
	bool CanBeHit() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
};