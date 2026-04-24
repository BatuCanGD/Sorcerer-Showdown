#pragma once
#include "CursedTool.h"

class Katana : public CursedTool {
public:
	Katana();
	void UseTool(Character*, Character*) override;
};