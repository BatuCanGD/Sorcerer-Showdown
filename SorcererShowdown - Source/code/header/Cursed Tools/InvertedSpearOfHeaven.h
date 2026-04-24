#pragma once
#include "CursedTool.h"

class InvertedSpearofHeaven : public CursedTool {
private:
	double special_tool_damage = base_tool_damage * 2.25;
public:
	InvertedSpearofHeaven();
	void UseTool(Character*, Character*) override;
};