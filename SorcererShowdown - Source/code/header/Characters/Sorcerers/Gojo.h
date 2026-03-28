#pragma once
#include "Sorcerer.h"

class Gojo : public Sorcerer {
public:
	Gojo();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};