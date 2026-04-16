#include "Mahito.h"
#include "TransfiguredHuman.h"
#include "BattlefieldHeader.h"
#include "IdleTransfiguration.h"
#include "SelfEmbodimentOfPerfection.h"

import std;

Mahito::Mahito() : CursedSpirit(650.0, 5500.0, 40.0) {
	technique = std::make_unique<IdleTransfiguration>();
	domain = std::make_unique<SelfEmbodimentOfPerfection>();
	passive_health_regen = 35.0;
	black_flash_chance = 20;
}

std::unique_ptr<Character> Mahito::Clone() const {
	return std::make_unique<Mahito>();
}

std::string Mahito::GetSimpleName() const {
	return "Mahito";
}
std::string Mahito::GetName() const {
	return "\033[35mMahito\033[0m";
}
bool Mahito::CanBeHit() const {
	return true;
}

void Mahito::OnCharacterTurn(Character* unused, Battlefield& bf){
	std::println("my name is skyfall and i hate kaisen! take my transfigured human!");
	IdleTransfiguration* tf = dynamic_cast<IdleTransfiguration*>(this->GetTechnique());

	constexpr auto TRANSFIGURE = IdleTransfiguration::TransfigurationType::Transfigure;
	constexpr auto SUMMON = IdleTransfiguration::TransfigurationType::Summon;

	double weakest_hp_pr = 1.1;
	Character* weakest = nullptr;
	int tf_amount = 0;
	bool summon_humans = true;

	for (const auto& chr : bf.battlefield) {
		if (chr.get() == this) continue;
		if (chr.get()->GetSimpleName() == "Transfigured Human") {
			tf_amount++;
		}
		double character_pr = chr->GetCharacterHealth() / chr->GetCharacterMaxHealth();
		if (character_pr < weakest_hp_pr || !weakest) {
			weakest = chr.get();
			weakest_hp_pr = character_pr;
		}
		if (tf_amount == bf.battlefield.size() - 1) summon_humans = false;
	}
	if (tf->GetTFcount() > 0 && summon_humans) {
		tf->UseIdleTransfiguration(this, weakest, SUMMON, bf);
		return;
	}
	else if (!(this->HPMoreThanMax(0.40) && this->GetDomainUses() >= 5) && 
			 !(tf->BurntOut() && this->DomainActive())) 
	{
		this->ActivateDomain();
		return;
	}
	if (!tf->BurntOut() && weakest) {
		tf->UseIdleTransfiguration(this, weakest, TRANSFIGURE, bf);
		return;
	}
	this->Attack(weakest);
}

void Mahito::TickCharacterSpecialty() {
	if (this->GetCharacterHealth() != this->GetCharacterMaxHealth()) {
		this->Regen(passive_health_regen);
	}
}