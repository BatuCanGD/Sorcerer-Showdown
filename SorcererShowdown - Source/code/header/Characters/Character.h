#pragma once
#include <memory>
#include <string>
#include <vector>

class CursedTool;
struct Battlefield;

class Character { 
protected:
	static int global_id_counter;
	int unique_id;

	bool is_player = false;

	double health;
	double max_health;
	double previous_health;

	double cursed_energy;
	double max_cursed_energy;
	double ce_regen_efficiency;
	
	std::unique_ptr<CursedTool> cursed_tool = nullptr;
	std::vector<std::unique_ptr<CursedTool>> inventory_curse;

	int stun_duration = 3;

	const double base_attack_damage = 20.0;

	bool is_stunned = false;
	bool is_heavenly_restricted;
	bool is_invulnerable = false;

	double current_ce_reinforcement;
	double max_ce_reinforcement;
public:
	Character(double hp, double ce, double regen);

	virtual ~Character();

	virtual std::unique_ptr<Character> Clone() const = 0;
	
	CursedTool* GetTool() const;
	const std::vector<std::unique_ptr<CursedTool>>& GetCursedTools() const;

	void SetVulnerability(bool);

	void SetHealth(double h);
	void SetCursedEnergy(double c);
	void SetMaxCursedEnergy(double c);
	void SetCursedEnergyRegen(double c);

	bool IsThePlayer() const;
	void SetAsPlayer(bool);

	virtual void OnCharacterTurn(Character*, Battlefield&);

	void Taunt(Character* target) const;

	virtual void Attack(Character*);

	void Damage(double h);
	void DamageBypass(double h);
	void Regen(double h);
	virtual void SpendCE(double c);
	void RegenCE();
	double GetCEregen() const;
	void UpdatePreviousHP();
	void SetStunState(bool s);
	void ClearStunTime();

	bool CEMoreThanMax(double) const;
	bool HPMoreThanMax(double) const;

	double GetCharacterCE() const;
	double GetCharacterMaxCE() const;

	double GetCharacterHealth() const;
	double GetCharacterMaxHealth() const;
	double GetCharacterPreviousHealth() const;

	bool IsCharacterStunned() const;

	void CursedToolChoice(int);
	void EquipToolByName(const std::string& weaponname);

	virtual double GetDamageReinforcement()const; // this for damage checks
	double GetReinforcement()const; // for the value
	double GetMaxReinforcement()const;

	void SetMaxReinforcement(double);
	void SetCurrentReinforcement(double);
	void AddReinforcement(double);
	void TickReinforcement();

	virtual void TickCharacterSpecialty();

	int GetID() const;
	std::string GetNameWithID() const;
	static void ResetGlobalID() { global_id_counter = 0; }
	static void AddGlobalID(int i) { global_id_counter += i; }

	virtual bool IsaCurseUser() const;
	virtual bool IsaCursedSpirit() const;
	virtual bool IsaSorcerer() const;
	virtual bool IsPhysicallyGifted() const;
	virtual bool IsShikigami() const;

	virtual std::string GetSimpleName() const = 0;
	virtual std::string GetName() const = 0;
	virtual bool CanBeHit() const = 0;

	void AssignID();
	virtual bool CanBeAssignedID() const;
};