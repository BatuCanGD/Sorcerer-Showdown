#pragma once
#include "Character.h"
#include <string>
#include <memory>
#include <vector>

class Domain;
class Technique;
class Specials;
class Shikigami;

class CurseUser : public Character {
protected:
	std::unique_ptr<Domain> domain = nullptr;
	std::unique_ptr<Domain> counter_domain = nullptr;
	std::unique_ptr<Technique> technique = nullptr;
	std::unique_ptr<Specials> special = nullptr;
	std::vector<std::unique_ptr<Shikigami>> shikigami;

	bool domain_amplification_active = false;
	bool counter_domain_active = false;
	bool domain_active = false;
	bool is_strained = false;
	bool counter_on_cooldown = false;
	bool zone_ce_boost = false;

	int total_domain_uses = 0;
	int technique_burnout_time = 0;
	int burnout_time = 0;
	int active_domain_time = 0;
	int active_counter_time = 0;  // active domain is based on technique recovery speed
	int counter_recover_time = 0;//  counters need an actual timer with them
	int black_flash_chance = 5;  
	int the_zone_time = 0;

	const double black_flash_multiplier = 4.0;

	const int domain_limit = 5;
	const int max_counter_time = 3;
	const int max_counter_cooldown = 2;
	const int max_domain_time = 5;
	const int max_technique_burnout_time = 4;
public:
    CurseUser(double hp, double ce, double regen);
    virtual ~CurseUser();

	Technique* GetTechnique() const;
	Specials* GetSpecial() const;
	Domain* GetCounterDomain() const;
	Domain* GetDomain() const;


	std::string GetDAstatus() const;
	std::string GetDomainStatus() const;
	std::string GetCounterStatus() const;
	std::string GetReinforcementStatus() const;

	void SpendCE(double ce) override;
	void SpendCEdirect(double ce);

	void Attack(Character*) override;

    bool DomainActive() const;
    void ActivateDomain();
    void DeactivateDomain();
    void TickDomain();
    void DomainDrain();
	int GetDomainUses() const;

    bool CounterDomainActive() const;
    void ActivateCounterDomain();
    void DeactivateCounterDomain();

    bool DomainAmplificationActive() const;
    void SetAmplification(bool);

    void TickZone();
    void RecoverBurnout();
    void RecoverTechniqueBurnout(Technique*);

    Shikigami* ChooseShikigami(size_t) const;
    const std::vector<std::unique_ptr<Shikigami>>& GetShikigami() const;
    void TickShikigami();
    void CleanupShikigami();

	std::string GetSimpleName() const override;
	std::string GetName() const override;
	bool IsaCurseUser() const override;
	bool CanBeHit() const override;
};