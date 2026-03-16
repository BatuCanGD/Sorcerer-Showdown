#pragma once

#include "Character.h"
#include "CombatContext.h"
#include "Shikigami.h"
#include "Domain.h"
#include "Technique.h"

#include <memory>
#include <vector>
#include <string>

class Shikigami;
class Technique;
class Domain;
class CombatContext;

class Sorcerer : public Character {
protected:
	std::unique_ptr<Domain> domain = nullptr;
	std::unique_ptr<Technique> technique = nullptr;
	std::unique_ptr<CombatContext> special = nullptr;

	std::vector<std::unique_ptr<Shikigami>> shikigami;
	
	bool six_eyes = false;

	double base_attack_damage = 20.0;

	bool domain_active = false;
	bool is_player = false;
	const int domain_limit = 5;
	int total_domain_uses = 0;

	bool domain_amplification_active = false;
	enum class ReverseCT {
		Disabled,
		Active,
		Overdrive
	};
	ReverseCT rct_state = ReverseCT::Disabled;
public:
	Sorcerer(double hp, double ce, double re) : Character(hp, ce, re) {}

	bool DomainActive() const;

	Domain* GetDomain();
	Technique* GetTechnique();
	CombatContext* GetSpecial();

	const std::vector<std::unique_ptr<Shikigami>>& GetShikigami() const;
	void SetAmplification(bool);
	void SetSixEyes(bool);
	bool HasSixEyes() const;

	bool IsThePlayer() const;
	void SetAsPlayer(bool);

	void SpendCE(double ce) override;
	void CheckSpecial(Sorcerer*);
	void DisableRCT();
	void EnableRCT();
	void BoostRCT();
	void DeactivateDomain();
	void ActivateDomain();
	void Attack(Character*);
	void DomainDrain();
	std::string GetName() const override;
	virtual void OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>&) = 0;
	virtual ~Sorcerer() = default;
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
protected:
	enum class Understanding {
		None,
		DomainAmplification,
		DomainAmpAndMahoraga
	};
	Understanding techknow = Understanding::None;
public:
	Sukuna();
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