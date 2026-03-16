#pragma once
#include <string>

class Sorcerer;

class Character { 
protected:
	double health = 100.0;
	double max_health;
	double previous_health;
	double cursed_energy = 500.0;
	const double max_cursed_energy;
	double ce_regen_efficiency = 2.5;
	bool is_stunned = false;
	int stun_duration = 2;
	bool is_heavenly_restricted = false;

	bool is_invulnerable = false;

	int GetRandomNumber(int min, int max);
public:

	Character(double hp, double ce, double regen);
	virtual ~Character() = default;
	
	void SetVulnerability(bool);

	void SetHealth(double h);
	void SetCursedEnergy(double c);
	//////////////////////////////////////
	void Damage(double h);
	void DamageBypass(double h);
	void Regen(double h);
	virtual void SpendCE(double c);
	void RegenCE();
	void UpdatePreviousHP();
	void SetStunState(bool s);
	void ClearStunTime();

	double GetCharacterCE() const;
	double GetMaxCharCE() const;

	double GetCharacterHealth() const;
	double GetCharacterMaxHealth() const;
	double GetCharacterPreviousHealth() const;

	bool IsCharacterStunned() const;
	bool IsHeavenlyRestricted() const;

	virtual std::string GetName() const;
	virtual bool CanBeHit() const = 0;
};