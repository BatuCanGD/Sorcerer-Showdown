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
}

bool Mahoraga::FullyAdapted() const {
    return InfStage == InfinityAdaptation::FourthSpin;
}

void Mahoraga::OnShikigamiTurn(Sorcerer* user) {
    if (!IsActive()) {
        Regen(shadow_health_regen);
        return;
    }
	if (user->GetCharacterCE() < keep_active_cost) {
        std::println("Mahoraga cannot maintain its active state due to insufficient Cursed Energy! It withdraws back into the shadows");
        Withdraw();
        return;
    }
    ActiveTimeIncrementor();
    Adapt();
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
        user->SpendCE(summon_amount);
    }
}

void Agito::OnShikigamiTurn(Sorcerer* user) {
    if (!IsActive()) {
        Regen(shadow_health_regen);
        return;
    }
	if (user->GetCharacterCE() < summon_amount) {
        std::println("Agito cannot maintain its support due to insufficient Cursed Energy! It withdraws back into the shadows");
        Withdraw();
        return;
    }
    ActiveTimeIncrementor();
	PassiveSupport(user);
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
    std::println("the carrier");
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