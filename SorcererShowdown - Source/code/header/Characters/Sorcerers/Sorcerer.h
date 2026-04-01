#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Character.h"

class Shikigami;
class Technique;
class CursedTool;
class Domain;
class Specials;

class Sorcerer : public Character {
protected:
	static int global_id_counter;
	int unique_id;

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
	virtual ~Sorcerer();
	Sorcerer(double hp, double ce, double re);
	virtual std::unique_ptr<Sorcerer> Clone() const = 0;

	Shikigami* ChooseShikigami(size_t) const;
	Technique* GetTechnique() const;
	Specials* GetSpecial() const;
	CursedTool* GetTool() const;
	Domain* GetCounterDomain() const;
	Domain* GetDomain() const;

	const std::vector<std::unique_ptr<Shikigami>>& GetShikigami() const;
	const std::vector<std::unique_ptr<CursedTool>>& GetCursedTools() const;

	void SetAmplification(bool);
	void SetSixEyes(bool);
	bool HasSixEyes() const;

	bool IsThePlayer() const;
	void SetAsPlayer(bool);

	void SpendCE(double ce) override;
	void SpendCEdirect(double ce);

	int GetDomainUses() const;

	void DisableRCT();
	void EnableRCT();
	void BoostRCT();

	std::string GetRCTstatus() const;
	std::string GetDAstatus() const;
	std::string GetDomainStatus() const;
	std::string GetCounterStatus() const;

	void UseRCT();
	bool DomainAmplificationActive() const;

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
	
	std::string GetSimpleName() const override;
	std::string GetName() const override;

	virtual void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) = 0;
	void RecoverBurnout();
	void RecoverTechniqueBurnout(Technique*);
	
	int GetID() const;
	std::string GetNameWithID() const;
	static void ResetGlobalID() { global_id_counter = 0; }
	static void AddGlobalID(int i) { global_id_counter += i; }

	void Taunt(Character* target);
	bool CanBeHit() const override;
};
