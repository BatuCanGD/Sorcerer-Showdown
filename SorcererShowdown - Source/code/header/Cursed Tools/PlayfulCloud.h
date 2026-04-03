#pragma once
#include "CursedTool.h"

class PlayfulCloud : public CursedTool {
public:
	void UseTool(Character*, Character*) override;
	std::string GetName() const override;
	std::string GetSimpleName() const override;
};