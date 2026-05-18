#pragma once
#include "code/header/CursedTools/CursedTool.h"

class Katana : public CursedTool {
protected:
	double base_damage = 25.0;
public:
	Katana();
	std::unique_ptr<CursedTool> Clone() const override;
	void UseTool(Character*, Character*) override;
};