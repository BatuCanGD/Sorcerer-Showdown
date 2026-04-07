#pragma once
#include "Techniques.h"

class CurseUser;
class Character;

class Limitless : public Technique { 
protected:
	bool Infinity = true;
	bool unlimited_hollow_purple_allowed = false;
	int blue_used_amount = 0;
	int red_used_amount = 0;
	int purple_used_amount = 0;
	static constexpr double blue_output = 20.0 + base_output;
	static constexpr double red_output = 40.0 + base_output;
	static constexpr double purple_output = 90.0 + base_output;
	static constexpr double unlpurple_output = 1500.0;
public:
	std::unique_ptr<Technique> Clone() const override;

	enum class LimitlessType { Blue = 1, Red = 2, Purple = 3 };

	void SetInfinity(bool s);
	void InfinityNerf(CurseUser*);
	bool CheckInfinity() const;

	void SetUnlimitedHollow(bool);
	bool UnlimitedHollowAllowed()const;

	int GetUsedBlueAmount()const;
	int GetUsedRedAmount()const;
	int GetUsedPurpleAmount()const;
	
	void UseUnlimitedHollowPurple(CurseUser* user, const std::vector<std::unique_ptr<Character>>& battlefield);
	void UseBlue(CurseUser* user, Character* target);
	void UseRed(CurseUser* user, Character* target);
	void UsePurple(CurseUser* user, Character* target);

	void UseTheLimitlessTechnique(LimitlessType choice, CurseUser* s, Character* c);
	void TechniqueMenu(CurseUser* user, Character* target, const std::vector<std::unique_ptr<Character>>& battlefield) override;
	void TechniqueSetting(CurseUser*, const std::vector<std::unique_ptr<Character>>& battlefield) override;

	void Chant() override;
	std::string GetTechniqueName() const override;
	std::string GetTechniqueSimpleName() const override;
};
