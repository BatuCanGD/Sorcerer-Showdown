#pragma once
#include "CursedSpirit.h"
#include <vector>
#include <string>

class TransfiguredHuman : public CursedSpirit {
public:
	TransfiguredHuman();
	void OnCharacterTurn(Character*, Battlefield&) override;
};