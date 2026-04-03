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

	void UseCleave(Sorcerer* user, Character* target);
	void UseDismantle(Sorcerer* user, Character* target);
	void UseTheWorldCuttingSlash(Sorcerer* user, Character* target);

	void UseShrineTechnique(ShrineType choice, Sorcerer* s, Character* c);
	void TechniqueMenu(Sorcerer* user, Character* target, const std::vector<std::unique_ptr<Character>>& battlefield) override;
	void TechniqueSetting(Sorcerer*, const std::vector<std::unique_ptr<Character>>& battlefield) override;

	std::string GetTechniqueName() const override;
	std::string GetTechniqueSimpleName() const override;
	void Chant() override;
};
