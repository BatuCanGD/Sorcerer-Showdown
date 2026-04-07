#pragma once
#include "Techniques.h"

class CurseUser;
class Character;

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

	void UseCleave(CurseUser* user, Character* target);
	void UseDismantle(CurseUser* user, Character* target);
	void UseTheWorldCuttingSlash(CurseUser* user, Character* target);

	void UseShrineTechnique(ShrineType choice, CurseUser* s, Character* c);
	void TechniqueMenu(CurseUser* user, Character* target, const std::vector<std::unique_ptr<Character>>& battlefield) override;
	void TechniqueSetting(CurseUser*, const std::vector<std::unique_ptr<Character>>& battlefield) override;

	std::string GetTechniqueName() const override;
	std::string GetTechniqueSimpleName() const override;
	void Chant() override;
};
