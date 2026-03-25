#include "Shikigami.h"
#include "Sorcerer.h"

import std;

// --- Base Shikigami  ---

Shikigami::Shikigami(double hp, double ce, double re) : Character(hp, ce, re) {
}

void Shikigami::PartiallyManifest() {
    shikigami_state = State::Partial;
}

void Shikigami::Manifest() {
    shikigami_state = State::Full;
}

void Shikigami::Withdraw() {
    shikigami_state = State::Shadow;
}

void Shikigami::ActiveTimeIncrementor() {
    if (!IsActive()) return;
    active_turn_amount++;
}

bool Shikigami::IsActive() const {
    return shikigami_state == State::Partial ||
            shikigami_state == State::Full;
}

bool Shikigami::IsActivePhysically() const {
    return shikigami_state == State::Full;
}

std::string Shikigami::GetShikigamiStatus() const {
    switch (shikigami_state) {
    case State::Shadow: return "Dormant";
    case State::Partial: return "Ability Active";
    case State::Full: return "Physically Manifested";
    default: return "Dormant";
    }
}


std::string Shikigami::GetName() const {
    return "Shikigami";
}

bool Shikigami::CanBeHit() const {
    return true;
}

// ------------- Mahoraga --------------

Mahoraga::Mahoraga() : Shikigami(400.0, 1000.0, 50.0) {}

void Mahoraga::Adapt() {
    if (!IsActive()) return;

    if (active_turn_amount >= 20) {
        InfStage = InfinityAdaptation::FourthSpin;
    }
    else if (active_turn_amount >= 15) {
        InfStage = InfinityAdaptation::ThirdSpin;
    }
    else if (active_turn_amount >= 10) {
        InfStage = InfinityAdaptation::SecondSpin;
    }
    else if (active_turn_amount >= 5) {
        InfStage = InfinityAdaptation::FirstSpin;
    }
    else {
        InfStage = InfinityAdaptation::None;
    }

    switch (InfStage) {
    case InfinityAdaptation::None: break;
    case InfinityAdaptation::FirstSpin: std::println("Mahoraga has started to adapt space itself!"); break;
    case InfinityAdaptation::SecondSpin: std::println("Mahoraga is on its second spin to adapt space itself!"); break;
    case InfinityAdaptation::ThirdSpin: std::println("Mahoraga is on its final spin to adapt space itself!"); break;
    case InfinityAdaptation::FourthSpin: std::println("Mahoraga has adapted to space itself!"); break;
    default:
        break;
    }
}

bool Mahoraga::FullyAdapted() const {
    return InfStage == InfinityAdaptation::FourthSpin;
}

void Mahoraga::OnShikigamiTurn(Sorcerer* user) {
    if (!IsActive()) {
        this->Regen(shadow_health_regen);
        return;
    }
	if (user->GetCharacterCE() < keep_active_cost) {
        std::println("Mahoraga cannot maintain its active state due to insufficient Cursed Energy! It withdraws back into the shadows");
        this->Withdraw();
        return;
    }
    this->ActiveTimeIncrementor();
    this->Adapt();
	user->SpendCE(keep_active_cost); 
}
std::string Mahoraga::GetName() const {
    return "Mahoraga";
}

bool Mahoraga::CanBeHit() const {
    if (IsActivePhysically()) {
        return true;
    }
    return false;
}


// ---------- Agito ----------

Agito::Agito() : Shikigami(150.0, 500.0, 20.0) {}

std::string Agito::GetName() const {
    return "Agito";
}

void Agito::PassiveSupport(Sorcerer* user) {
    if (IsActive()) {
        user->Regen(passive_heal_amount);
        std::println("Agito has healed {}", user->GetName());
    }
}

void Agito::OnShikigamiTurn(Sorcerer* user) {
    if (!IsActive()) {
        this->Regen(shadow_health_regen);
        return;
    }
	if (user->GetCharacterCE() < summon_amount) {
        std::println("Agito cannot maintain its support due to insufficient Cursed Energy! It withdraws back into the shadows");
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

// ----------- Rika -----------

Rika::Rika() : Shikigami(1000.0, 100000.0, 500.0) {}


void Rika::OnShikigamiTurn(Sorcerer* user) {
    if (IsActivePhysically()) {
        if (active_turn_amount > 5) {
            std::println("the queen of curses has reached her time limit\nRika orimoto trudges back into the shadows!");
            this->Withdraw();
            RikaCooldownRegeneration(user);
            return;
        }
        if (!value_saved) {
            SaveUserCursedEnergy(user);
        }
        user->SetMaxCursedEnergy(rika_ce_increase);
        user->SetCursedEnergyRegen(rika_regen_increase);

        ActiveTimeIncrementor();
        return;
    }
    RikaCooldownRegeneration(user);
}

void Rika::RikaCooldownRegeneration(Sorcerer* user) {
    if (!IsActivePhysically()) {
        if (active_turn_amount > 5) {
            active_cooldown--;
            if (active_cooldown <= 0) {
                active_turn_amount = 0;
                active_cooldown = 5;
            }
        }
        user->SetMaxCursedEnergy(user_ce);
        user->SetCursedEnergyRegen(user_regen);
        if (user->GetCharacterCE() > user->GetMaxCharCE()) {
            user->SetCursedEnergy(user->GetMaxCharCE());
        }
    }
}

void Rika::SaveUserCursedEnergy(Sorcerer* user) {
    if (value_saved) return;
    user_ce = user->GetMaxCharCE();
    user_regen = user->GetCEregen();
    value_saved = true;
}

bool Rika::CanBeHit() const {
    if (IsActivePhysically()) {
        return true;
    }
    return false;
}


std::string Rika::GetName() const {
    return "Rika";
}