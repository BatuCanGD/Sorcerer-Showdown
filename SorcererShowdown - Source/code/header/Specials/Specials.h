#pragma once
#include <string>

class CurseUser;

class Specials {
public:
	virtual ~Specials() = default;
	virtual void PerformSpecial(CurseUser* user) = 0;
	virtual std::string GetSpecialSimplifiedName() const = 0;
};