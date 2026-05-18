#pragma once
#include "code/header/CursedTools/CursedTool.h"

class InvertedSpearofHeaven : public CursedTool {
private:
	double tool_damage = 10.0 * 2.25;
public:
	InvertedSpearofHeaven();
	std::unique_ptr<CursedTool> Clone() const override;
	void UseTool(Character*, Character*) override;
	bool IsAntiTechniqueWeapon() const override;
};