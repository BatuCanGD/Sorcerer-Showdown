#pragma once
#include <string>

class Sorcerer;

class Specials {
public:
	virtual ~Specials() = default;
	virtual void PerformSpecial(Sorcerer* user) = 0;
	virtual std::string GetSpecialSimplifiedName() const = 0;
};

class WorldCuttingSlash : public Specials {
public:	
	void PerformSpecial(Sorcerer* user) override;
	std::string GetSpecialSimplifiedName() const override;
};