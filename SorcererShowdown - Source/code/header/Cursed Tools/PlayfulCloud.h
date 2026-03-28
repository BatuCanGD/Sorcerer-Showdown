#pragma once
#include "CursedTool.h"

class PlayfulCloud : public CursedTool {
private:
	double GetCalculatedStrength(Sorcerer*) const;
public:
	void UseTool(Sorcerer*, Character*) override;
	std::string GetName() const override;
};