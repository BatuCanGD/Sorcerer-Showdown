#pragma once
#include <string>

using namespace std;

class Sorcerer;

class Character { 
protected:
	double health = 100.0;
	double cursed_energy = 500.0;
	bool is_stunned = false;
	int stun_duration = 2;
	bool is_heavenly_restricted = false;

	enum class AttackType {
		None,
		Basic,
		Technique,
		Domain
	};
	enum class BlockState {
		None,
		Blocking,
	};
	AttackType current_attack = AttackType::None;
	BlockState block_state = BlockState::None;
public:

	Character(double hp, double ce);
	virtual ~Character() = default;
	
	void Damage(double h);
	void Regen(double h);
	void SpendCE(double c);
	void RegenCE(double c);
	void SetStunState(bool s);
	void ClearStunTime();
	//////////////////////////////////////
	void SetAttack();
	void SetUsingTechnique();
	void SetUsingDomain();
	void SetBlocking();

	bool IsAttacking() const;
	bool IsUsingTechnique() const;
	bool IsUsingDomain() const;
	bool IsBlocking() const;

	void ResetAttackState();
	void ResetBlockState();
	//////////////////////////////////////

	double GetCharacterCE() const;
	double GetCharacterHealth() const;

	bool IsCharacterStunned() const;
	bool IsHeavenlyRestricted() const;

	virtual string GetName() const;
};