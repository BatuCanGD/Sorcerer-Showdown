#include "Fighting.h"
#include "Sorcerer.h"
#include "Character.h"

#include <print>

void FightActions::Attack(Character* user, Character* target) {
	if (user->IsAttacking()) {
		if (target->IsBlocking()) {
			std::println("The attack was blocked!");
			user->ResetAttackState();
			return;
		}
		else if (!target->CanBeHit()) {
			std::println("The attack was stopped!");
			user->ResetAttackState();
			return;
		}
		std::println("{} strikes at {}!", user->GetName(), target->GetName());
		target->Damage(basic_attack_damage);
		user->ResetAttackState();
	}
	else {
		std::println("{} prepares to strike at {}!", user->GetName(), target->GetName());
		user->SetAttack();
	}

}
void FightActions::UseTechnique(Sorcerer* user, Character* target) {
	Technique* user_technique = user->GetTechnique();
	if (user_technique == nullptr) return;

	if (user->IsUsingTechnique()) {
		user->ResetAttackState();
	}
	else {
		user->SetUsingTechnique();
	}


}
void FightActions::CheckDomain(Sorcerer* user) {
	if (user->DomainActive()) {
		user->SetUsingDomain();
	}
	else {
		user->ResetAttackState();
	}
}
void FightActions::Block(Character* user) {
	if (user->IsBlocking()) {
		user->ResetBlockState();
	}
	else {
		user->SetBlocking();
	}

}