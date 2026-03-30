#include "Agito.h"
#include "Sorcerer.h"

import std;

Agito::Agito() : Shikigami(150.0, 500.0, 20.0) {}

std::string Agito::GetName() const {
    return "\033[95mAgito\033[0m";
}

void Agito::PassiveSupport(Sorcerer* user) {
    if (IsActive()) {
        user->Regen(passive_heal_amount);
        std::println("{} has healed {}",this->GetName(), user->GetName());
    }
}

void Agito::OnShikigamiTurn(Sorcerer* user) {
    if (!IsActive()) {
        this->Regen(shadow_health_regen);
        return;
    }
    if (user->GetCharacterCE() < summon_amount) {
        std::println("{} cannot maintain its support due to insufficient Cursed Energy! It withdraws back into the shadows",this->GetName());
        this->Withdraw();
        return;
    }
    ActiveTimeIncrementor();
    PassiveSupport(user);
    user->SpendCE(summon_amount);
}

bool Agito::CanBeHit() const {
    if (IsActivePhysically()) {
        return true;
    }
    return false;
}