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

using namespace std;

class Sorcerer : public Character {
protected:
	unique_ptr<Domain> domain = nullptr;
	unique_ptr<Technique> technique = nullptr;
	vector<unique_ptr<Shikigami>> shikigami;

	bool domain_active = false;
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
	const vector<unique_ptr<Shikigami>>& GetShikigami() const;

	void DisableRCT();
	void EnableRCT();
	void BoostRCT();
	void DeactivateDomain();
	void ActivateDomain(Character* user);
	string GetName() const override;
	virtual void OnSorcererTurn();
	virtual ~Sorcerer() = default;
};


class Gojo : public Sorcerer { // fighters
public:
	Gojo();
	string GetName() const override;
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
	string GetName() const override;
	void OnSorcererTurn() override;
};