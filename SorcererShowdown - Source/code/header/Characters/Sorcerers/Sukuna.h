#pragma once
#include "Sorcerer.h"

class Sukuna : public Sorcerer {
public:
	Sukuna();
	std::unique_ptr<Sorcerer> Clone() const override;
	std::string GetSimpleName() const override;
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};
