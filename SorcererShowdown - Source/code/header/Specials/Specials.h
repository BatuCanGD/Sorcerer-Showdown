#pragma once
#include <string>

class CurseUser;

class Specials {
protected:
	std::string special_name = "";
	std::string special_color = "";
public:
	virtual ~Specials() = default;
	virtual void PerformSpecial(CurseUser* user) = 0;
	virtual std::string GetSpecialName() const;
};