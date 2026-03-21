#pragma once
#include <string>

class Sorcerer;
class Character;

class CursedTool {
protected:
	double base_tool_damage = 20.0;
	double base_tool_support = 10.0;
	double special_tool_mult = 2.5;
public:
	virtual ~CursedTool() = default;
	virtual void UseTool(Sorcerer*, Character*) = 0;
	virtual std::string GetName() const = 0;
};

class InvertedSpearofHeaven : public CursedTool {
private:
	double special_tool_damage = base_tool_damage * 2.25;
public:
	void UseTool(Sorcerer*, Character*) override;
	std::string GetName() const override;
};

class PlayfulCloud : public CursedTool {
private:
	double GetCalculatedStrength(Sorcerer*);
public:
	void UseTool(Sorcerer*, Character*) override;
	std::string GetName() const override;
};

class Katana : public CursedTool {
public:
	void UseTool(Sorcerer*, Character*) override;
	std::string GetName() const override;
};