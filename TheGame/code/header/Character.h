#pragma once
#include <string>

class Sorcerer;

class Character { 
protected:
	double health;
	double max_health;
	double previous_health;

	double cursed_energy;
	double max_cursed_energy;
	double ce_regen_efficiency;

	int stun_duration;

	bool is_stunned;
	bool is_heavenly_restricted;
	bool is_invulnerable;
public:
	Character(double hp, double ce, double regen);
	virtual ~Character() = default;
	
	void SetVulnerability(bool);

	void SetHealth(double h);
	void SetCursedEnergy(double c);
	void SetMaxCursedEnergy(double c);
	void SetCursedEnergyRegen(double c);

	void Damage(double h);
	void DamageBypass(double h);
	void Regen(double h);
	virtual void SpendCE(double c);
	void RegenCE();
	double GetCEregen() const;
	void UpdatePreviousHP();
	void SetStunState(bool s);
	void ClearStunTime();

	double GetCharacterCE() const;
	double GetCharacterMaxCE() const;

	double GetCharacterHealth() const;
	double GetCharacterMaxHealth() const;
	double GetCharacterPreviousHealth() const;

	bool IsCharacterStunned() const;
	bool IsHeavenlyRestricted() const;

	virtual std::string GetName() const;
	virtual bool CanBeHit() const = 0;
};