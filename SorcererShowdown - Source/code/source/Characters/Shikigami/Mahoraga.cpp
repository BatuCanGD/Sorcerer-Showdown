#include "Mahoraga.h"
#include "Sorcerer.h"

import std;

Mahoraga::Mahoraga() : Shikigami(400.0, 1000.0, 50.0) {}

void Mahoraga::Adapt() {
    if (!IsActive()) return;

    if (active_turn_amount >= 40) {
        InfStage = InfinityAdaptation::FourthSpin;
    }
    else if (active_turn_amount >= 25) {
        InfStage = InfinityAdaptation::ThirdSpin;
    }
    else if (active_turn_amount >= 15) {
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
