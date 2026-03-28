#pragma once
#include "Sorcerer.h"

class Toji : public Sorcerer {
public:
	Toji();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};