#include "Agito.h"
#include "BattlefieldHeader.h"
#include "CurseUser.h"

import std;

Agito::Agito() : Shikigami(150.0, 500.0, 20.0) {}

std::string Agito::GetName() const {
    return "\033[95mAgito\033[0m";
}
std::string Agito::GetSimpleName() const {
    return "Agito";
}

void Agito::PassiveSupport(Character* user) {
    if (IsActive()) {
        user->Regen(passive_heal_amount);
        std::println("{} has healed {}",this->GetName(), user->GetNameWithID());
    }
}

void Agito::OnCharacterTurn(Character* user, Battlefield& bf) {
    if (!IsActive()) {
        this->Regen(shadow_health_regen);
        return;
    }
    if (user->GetCharacterCE() < summon_amount) {
        std::println("{} cannot maintain its support for {} due to insufficient Cursed Energy! It withdraws back into the shadows",this->GetName(), user->GetNameWithID());
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

std::unique_ptr<Character> Agito::Clone() const {
    return nullptr;
}