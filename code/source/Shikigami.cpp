#include "Shikigami.h"
#include "Sorcerer.h"

// --- Base Shikigami  ---

Shikigami::Shikigami(double hp, double ce) : Character(hp, ce) {
}

void Shikigami::PartiallyManifest() {
    shikigami_stats = ShikigamiStatus::PartialManifestation;
}

void Shikigami::Manifest() {
    shikigami_stats = ShikigamiStatus::Manifested;
}

void Shikigami::Withdraw() {
    shikigami_stats = ShikigamiStatus::Shadow;
}

void Shikigami::ActiveTimeIncrementor() {
    if (!IsActive()) return;
    active_turn_amount++;
}

bool Shikigami::IsActive() const {
    return shikigami_stats == ShikigamiStatus::PartialManifestation ||
            shikigami_stats == ShikigamiStatus::Manifested;
}

std::string Shikigami::GetName() const {
    return "Shikigami";
}

// ------------- Mahoraga --------------

Mahoraga::Mahoraga() : Shikigami(300.0, 1000.0) {}

void Mahoraga::Adapt() {
    if (!IsActive()) return;

    if (active_turn_amount >= 50) {
        InfStage = InfinityAdaptation::FourthSpin;
    }
    else if (active_turn_amount >= 40) {
        InfStage = InfinityAdaptation::ThirdSpin;
    }
    else if (active_turn_amount >= 25) {
        InfStage = InfinityAdaptation::SecondSpin;
    }
    else if (active_turn_amount >= 10) {
        InfStage = InfinityAdaptation::FirstSpin;
    }
    else {
        InfStage = InfinityAdaptation::None;
    }
}

bool Mahoraga::FullyAdaptedToInfinity() const {
    return InfStage == InfinityAdaptation::FourthSpin;
}

void Mahoraga::OnShikigamiTurn() {
    ActiveTimeIncrementor();
    Adapt();
}
std::string Mahoraga::GetName() const {
    return "Mahoraga";
}

// ---------- Agito ----------

Agito::Agito() : Shikigami(200.0, 500.0) {}

void Agito::PassiveSupport(Sorcerer* user) {
    if (this->IsActive()) {
        user->Regen(passive_heal_amount);
        user->SpendCE(summon_amount);
    }

}
std::string Agito::GetName() const {
    return "Agito";
}
void Agito::OnShikigamiTurn() {
    ActiveTimeIncrementor();
}