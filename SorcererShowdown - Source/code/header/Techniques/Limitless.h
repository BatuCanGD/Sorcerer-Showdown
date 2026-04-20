#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class Limitless : public Technique { 
protected:
	bool Infinity = true;
	bool unlimited_hollow_purple_allowed = false;
	int blue_used_amount = 0;
	int red_used_amount = 0;
	int purple_used_amount = 0;
	static constexpr double blue_output = 60.0;
	static constexpr double red_output = 75.0;
	static constexpr double purple_output = 150.0;
	static constexpr double unlpurple_output = 1750.0;
public:
	std::unique_ptr<Technique> Clone() const override;

	void SetInfinity(bool s);
	void InfinityNerf(CurseUser*);
	bool CheckInfinity() const;

	void SetUnlimitedHollow(bool);
	bool UnlimitedHollowAllowed()const;

	int GetUsedBlueAmount()const;
	int GetUsedRedAmount()const;
	int GetUsedPurpleAmount()const;
	
	void UseUnlimitedHollowPurple(CurseUser* user, Battlefield&);
	void UseBlue(CurseUser* user, Character* target);
	void UseRed(CurseUser* user, Character* target);
	void UsePurple(CurseUser* user, Character* target);

	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	void TechniqueSetting(CurseUser*, Battlefield&) override;

	void Chant() override;
	std::string GetTechniqueName() const override;
	std::string GetTechniqueSimpleName() const override;
};
