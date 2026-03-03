#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Character.h"
#include "Technique.h"
#include "Domain.h"
#include "Shikigami.h"

class Shikigami;
class Technique;
class Domain;

class Sorcerer : public Character {
protected:
	std::unique_ptr<Domain> domain = nullptr;
	std::unique_ptr<Technique> technique = nullptr;
	std::vector<std::unique_ptr<Shikigami>> shikigami;

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
	Sorcerer(double hp, double ce) : Character(hp, ce) {}

	bool DomainActive() const;
	Domain* GetDomain();
	Technique* GetTechnique();
	const std::vector<std::unique_ptr<Shikigami>>& GetShikigami() const;
	bool IsDomainActive() const;

	bool IsThePlayer() const;
	void SetAsPlayer(bool);

	void DisableRCT();
	void EnableRCT();
	void BoostRCT();
	void DeactivateDomain();
	void ActivateDomain(Character* user);
	std::string GetName() const override;
	virtual void OnSorcererTurn();
	virtual ~Sorcerer() = default;
};


class Gojo : public Sorcerer { // fighters
public:
	Gojo();
	std::string GetName() const override;
	void OnSorcererTurn() override;
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
	void OnSorcererTurn() override;
};