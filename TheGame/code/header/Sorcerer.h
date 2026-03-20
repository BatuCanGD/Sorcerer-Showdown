#pragma once

#include "Character.h"
#include "Specials.h"
#include "Shikigami.h"
#include "Domain.h"
#include "Techniques.h"
#include "CursedTool.h"

#include <memory>
#include <vector>
#include <string>

class Shikigami;
class Technique;
class Domain;
class Specials;

class Sorcerer : public Character {
protected:
	std::unique_ptr<Domain> domain = nullptr;
	std::unique_ptr<Technique> technique = nullptr;
	std::unique_ptr<Specials> special = nullptr;
	std::unique_ptr<CursedTool> cursed_tool = nullptr;
	
	std::vector<std::unique_ptr<CursedTool>> inventory_curse; 
	std::vector<std::unique_ptr<Shikigami>> shikigami;
	
	bool six_eyes = false;

	double base_attack_damage = 20.0;

	bool domain_active = false;
	bool is_player = false;

	int total_domain_uses = 0;
	int burnout_time = 0;
	int active_domain_time = 0;

	int black_flash_chance = 5;
	int the_zone_time = 0;

	const double black_flash_multiplier = 4.0;

	const int domain_limit = 5;
	const int max_domain_time = 5;
	const int max_burnout_time = 2;

	bool domain_amplification_active = false;
	enum class ReverseCT {
		Disabled,
		Active,
		Overdrive
	};
	ReverseCT rct_state = ReverseCT::Disabled;
public:
	virtual ~Sorcerer() = default;
	Sorcerer(double hp, double ce, double re) : Character(hp, ce, re) {}

	enum class CurrentWeapon { None = 0, ISOH = 1, PLCLD = 2 };

	Domain* GetDomain();
	Technique* GetTechnique();
	Specials* GetSpecial();
	CursedTool* GetTool();

	const std::vector<std::unique_ptr<Shikigami>>& GetShikigami() const;
	const std::vector<std::unique_ptr<CursedTool>>& GetCursedTools() const;

	void SetAmplification(bool);
	void SetSixEyes(bool);
	bool HasSixEyes() const;

	bool IsThePlayer() const;
	void SetAsPlayer(bool);

	void SpendCE(double ce) override;

	int GetDomainUses() const;

	void DisableRCT();
	void EnableRCT();
	void BoostRCT();

	std::string GetRCTstatus() const;
	std::string GetDAstatus() const;

	void UseRCT();

	bool DomainActive() const;
	void DeactivateDomain();
	void ActivateDomain();
	void DomainDrain();

	void TickDomain();
	void TickZone();

	void CursedToolChoice(int);
	void ChangeCursedTool(CurrentWeapon);

	void Attack(Character*);
	
	void CleanupShikigami();
	
	std::string GetName() const override;

	virtual void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) = 0;
	void RecoverBurnout(Technique*);
	
	void Taunt(Character* target);
	bool CanBeHit() const override;
};


class Gojo : public Sorcerer { // fighters
public:
	Gojo();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};

class Sukuna : public Sorcerer {
public:
	Sukuna();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};

class Toji : public Sorcerer {
private:
	int INVERTED_SPEAR_OF_HEAVEN = 1;
	int PLAYFUL_CLOUD = 2;
public:
	Toji();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};




class test_sorcerer : public Sorcerer {
public:
	test_sorcerer();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};