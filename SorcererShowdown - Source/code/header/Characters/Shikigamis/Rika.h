#pragma once
#include "Shikigami.h"

class Rika : public Shikigami {
private:
	bool value_saved = false;
	double user_ce;
	double user_regen;
	static constexpr double rika_ce_increase = 100000.0;
	static constexpr double rika_regen_increase = 10000.0;
	static constexpr int max_active_time = 6;
	int active_cooldown = 5;
public:
	Rika();
	void OnShikigamiTurn(Sorcerer*) override;
	void SaveUserCursedEnergy(Sorcerer*);
	void RikaCooldownRegeneration(Sorcerer*);
	std::string GetName() const override;
	bool CanBeHit() const override;
};