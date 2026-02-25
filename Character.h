#pragma once

class Character { 
protected:
	double health = 100.0;
	double cursed_energy = 500.0;
	bool is_stunned = false;
	int stun_duration = 2;
	bool is_heavenly_restricted = false;
public:
	
	Character(double hp, double ce);
	virtual ~Character() = default;
	
	void Damage(double h);
	void Regen(double h);
	void SpendCE(double c);
	void RegenCE(double c);
	void SetStunState(bool s);
	void ClearStunTime();

	double GetCharacterCE() const;
	double GetCharacterHealth() const;

	bool IsCharacterStunned() const;
	bool IsHeavenlyRestricted() const;
	
};