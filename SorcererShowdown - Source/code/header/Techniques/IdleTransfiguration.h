#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class IdleTransfiguration : public Technique {
protected:
	int transfigured_human_count = 3;
	const double transfiguration_damage = 100.0;
public:
	std::string GetTechniqueName() const override;
	std::string GetTechniqueSimpleName() const override;

	enum class TransfigurationType { Transfigure = 1, Summon = 2 };

	void UseIdleTransfiguration(CurseUser* user, Character*, TransfigurationType type, Battlefield&);

	void UseTransfiguration(CurseUser* user, Character* target);
	void SummonTransfiguredHumans(Battlefield&);

	int GetTFcount() const;

	void Chant() override;
	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	void TechniqueSetting(CurseUser*, Battlefield&) override;
	std::unique_ptr<Technique> Clone() const override;
};