#pragma once
#include <string>

class Sorcerer;
class Character;

class CursedTool {
protected:
	double base_tool_damage = 20.0;
	double base_tool_support = 10.0;
	double special_tool_mult = 2.5;

	double GetCalculatedStrength(Character*) const;
public:
	virtual ~CursedTool() = default;
	virtual void UseTool(Character*, Character*) = 0;
	virtual std::string GetName() const = 0;
	virtual std::string GetSimpleName() const = 0;
};
