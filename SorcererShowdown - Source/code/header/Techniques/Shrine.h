#pragma once
#include "Techniques.h"

class Shrine : public Technique { 
protected:
	static constexpr double slash_output = 45.0;
	static constexpr double cleave_output = 300.0;
	static constexpr double wcs_output = 2500.0;
	bool world_cutting_slash_allowed = false;
public:
	std::unique_ptr<Technique> Clone() const override;

	enum class ShrineType { Dismantle = 1, Cleave = 2, WCS = 3 };

	void SetWCS(bool s);
	bool WorldCuttingSlashUnlocked() const;
	std::string GetTechniqueName() const override;

	void CleaveTechniqueDamageTarget(Sorcerer* user, Character* target);
	void DismantleTechniqueDamageTarget(Sorcerer* user, Character* target);
	void WorldCuttingSlashToTarget(Sorcerer* user, Character* target);
	void UseShrineTechnique(ShrineType choice, Sorcerer* s, Character* c);
	void TechniqueMenu(Sorcerer* user, Character* target, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) override;
	void TechniqueSetting(Sorcerer*, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) override;
	void Chant() override;
};
