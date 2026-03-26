#include "Shikigami.h"
#include "Sorcerer.h"

import std;

Rika::Rika() : Shikigami(INT32_MAX, 100000.0, 3000.0) {}

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
    if (!IsActivePhysically() && value_saved) {
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