#pragma once
class Character;
class Sorcerer;

class FightActions {
protected:
	static constexpr double basic_attack_damage = 20.0;
public:
	void Attack(Character* user, Character* target);
	void UseTechnique(Sorcerer* user, Character* target);
	void CheckDomain(Sorcerer* user);
	void Block(Character* user);
};