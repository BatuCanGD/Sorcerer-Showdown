#pragma once
#include "Sorcerer.h"

class Sukuna : public Sorcerer {
public:
	Sukuna();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};
