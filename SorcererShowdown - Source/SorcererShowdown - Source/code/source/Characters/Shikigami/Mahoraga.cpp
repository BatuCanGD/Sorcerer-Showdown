#include "Mahoraga.h"
#include "Sorcerer.h"
#include "Utils.h"

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
    case InfinityAdaptation::FirstSpin: std::println("{} has started to adapt space itself!", this->GetName()); break;
    case InfinityAdaptation::SecondSpin: std::println("{} is on its second spin to adapt space itself!", this->GetName()); break;
    case InfinityAdaptation::ThirdSpin: std::println("{} is on its final spin to adapt space itself!", this->GetName()); break;
    case InfinityAdaptation::FourthSpin: std::println("{} has adapted to space itself!",this->GetName()); break;
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
        std::println("{} cannot maintain its active state due to insufficient {}Cursed Energy!{} It withdraws back into the shadows",this->GetName(), Color::Cyan,Color::Clear);
        this->Withdraw();
        return;
    }
    this->ActiveTimeIncrementor();
    this->Adapt();
    user->SpendCE(keep_active_cost);
}
std::string Mahoraga::GetName() const {
    return "\033[33mMahoraga\033[0m";
}

bool Mahoraga::CanBeHit() const {
    if (IsActivePhysically()) {
        return true;
    }
    return false;
}
