#pragma once
#include "CursedTool.h"

class Katana : public CursedTool {
public:
	void UseTool(Sorcerer*, Character*) override;
	std::string GetName() const override;
	std::string GetSimpleName() const override;
};