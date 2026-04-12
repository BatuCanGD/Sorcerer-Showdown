#pragma once
#include "Shikigami.h"

class Rika : public Shikigami {
private:
	bool value_saved = false;
	double user_ce;
	double user_regen;
	static constexpr double rika_ce_increase = 100000.0;
	static constexpr double rika_regen_increase = 20000.0;
	static constexpr int max_active_time = 5;
	int active_cooldown = 5;
public:
	Rika();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, std::vector<std::unique_ptr<Character>>&) override;
	void SaveUserCursedEnergy(Character*);
	void RikaCooldownRegeneration(Character*);

	std::string GetSimpleName() const override;
	std::string GetName() const override;
	bool CanBeHit() const override;
};