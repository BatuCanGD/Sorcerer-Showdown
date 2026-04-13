#include "Mahito.h"
#include "TransfiguredHuman.h"
#include "BattlefieldHeader.h"

import std;

Mahito::Mahito() : CursedSpirit(650.0, 5500.0, 40.0) {
	passive_health_regen = 40.0;
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
	bf.spawn_queue.push_back(std::make_unique<TransfiguredHuman>());
}

void Mahito::TickCharacterSpecialty() {
	if (this->GetCharacterHealth() != this->GetCharacterMaxHealth()) {
		this->Regen(passive_health_regen);
	}
}