#pragma once
#include "CursedSpirit.h"
#include <vector>
#include <string>

class Mahito : public CursedSpirit {
public:
	Mahito();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
};