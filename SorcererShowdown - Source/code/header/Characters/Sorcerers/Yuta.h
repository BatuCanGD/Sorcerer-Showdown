#pragma once
#include "Sorcerer.h"

class Yuta : public Sorcerer {
public:
	Yuta();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};