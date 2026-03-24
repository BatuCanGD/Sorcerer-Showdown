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
	std::unique_ptr<Domain> counter_domain = nullptr; // frankenstein of a class
	std::unique_ptr<Technique> technique = nullptr;
	std::unique_ptr<Specials> special = nullptr; // cursed technique extension <- why didnt i think of this name first??
	std::unique_ptr<CursedTool> cursed_tool = nullptr;
	
	std::vector<std::unique_ptr<CursedTool>> inventory_curse; 
	std::vector<std::unique_ptr<Shikigami>> shikigami;
	
	bool six_eyes = false;

	double base_attack_damage = 20.0;

	bool domain_amplification_active = false;

	bool counter_domain_active = false;
	bool domain_active = false;

	bool is_player = false;

	bool is_strained = false;

	int total_domain_uses = 0;
	int technique_burnout_time = 0;
	int burnout_time = 0;

	int active_domain_time = 0;
	int active_counter_time = 0;

	int black_flash_chance = 5;
	int the_zone_time = 0;

	const double black_flash_multiplier = 4.0;

	const int domain_limit = 5;
	const int max_counter_time = 3;
	const int max_domain_time = 5;
	const int max_technique_burnout_time = 4;

	enum class ReverseCT {
		Disabled, Active, Overdrive
	};
	ReverseCT rct_state = ReverseCT::Disabled;
public:
	virtual ~Sorcerer() = default;
	Sorcerer(double hp, double ce, double re) : Character(hp, ce, re) {}

	Shikigami* ChooseShikigami(size_t);
	Technique* GetTechnique();
	Specials* GetSpecial();
	CursedTool* GetTool();
	Domain* GetCounterDomain();
	Domain* GetDomain();

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

	bool CounterDomainActive() const;
	void ActivateCounterDomain();
	void DeactivateCounterDomain();

	void TickDomain();
	void TickZone();
	void TickShikigami();

	void CursedToolChoice(int);
	void EquipToolByName(const std::string& weaponname);

	void Attack(Character*);
	
	void CleanupShikigami();
	
	std::string GetName() const override;

	virtual void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) = 0;
	void RecoverBurnout();
	void RecoverTechniqueBurnout(Technique*);
	
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
public:
	Toji();
	std::string GetName() const override;
	void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) override;
	bool CanBeHit() const override;
};

class Yuta : public Sorcerer {
private:
	static constexpr double rika_ce_increase = 30000.0;
	static constexpr double rika_regen_increase = 150.0;
public:
	Yuta();
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