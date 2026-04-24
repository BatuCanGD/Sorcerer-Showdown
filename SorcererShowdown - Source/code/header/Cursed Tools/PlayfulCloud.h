#pragma once
#include "CursedTool.h"

class PlayfulCloud : public CursedTool {
public:
	PlayfulCloud();
	void UseTool(Character*, Character*) override;
};