#pragma once
#include "Specials.h"

class WorldCuttingSlash : public Specials {
public:
	void PerformSpecial(Sorcerer* user) override;
	std::string GetSpecialSimplifiedName() const override;
};