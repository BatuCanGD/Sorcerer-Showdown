#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class Shrine : public Technique { 
protected:
	static constexpr double slash_output = 60.0;
	static constexpr double cleave_output = 300.0;
	static constexpr double wcs_output = 2500.0;
	bool world_cutting_slash_allowed = false;
public:
	std::unique_ptr<Technique> Clone() const override;

	void SetWCS(bool s);
	bool WorldCuttingSlashUnlocked() const;

	void UseCleave(CurseUser* user, Character* target);
	void UseDismantle(CurseUser* user, Character* target);
	void UseTheWorldCuttingSlash(CurseUser* user, Character* target);

	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	void TechniqueSetting(CurseUser*, Battlefield&) override;

	std::string GetTechniqueName() const override;
	std::string GetTechniqueSimpleName() const override;
	void Chant() override;
};
