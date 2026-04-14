#include "Sorcerer.h"
#include "CurseUser.h"
#include "Limitless.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Domain.h"
#include "CursedTool.h"
#include "Utils.h"
#include "Specials.h"

import std;

Sorcerer::Sorcerer(double hp, double ce, double re) : CurseUser(hp, ce, re) {}
Sorcerer::~Sorcerer() = default;

void Sorcerer::SetSixEyes(bool t) {
    six_eyes = t;
}

bool Sorcerer::HasSixEyes() const {
    return six_eyes;
}

void Sorcerer::SpendCE(double ce) {
    double efficiency = 1.0;
    if (HasSixEyes()) {
        efficiency = 0.3;
        if (technique && technique->GetStatus() == Technique::Status::BurntOut) {
            efficiency = 0.85; 
        }
    }
    cursed_energy = std::max(cursed_energy - (ce * efficiency), 0.0);
}

void Sorcerer::DisableRCT() {
    rct_state = ReverseCT::Disabled;
}

void Sorcerer::EnableRCT() {
    rct_state = ReverseCT::Active;
}

void Sorcerer::BoostRCT() {
    rct_state = ReverseCT::Overdrive;
}

std::string Sorcerer::GetRCTstatus() const {
    switch (rct_state) {
    case ReverseCT::Disabled: return "\033[31mDisabled\033[0m";
    case ReverseCT::Active: return "\033[33mActive\033[0m";
    case ReverseCT::Overdrive: return "\033[32mOverdrive\033[0m";
    default: return "\033[2;90mDisabled\033[0m";
    }
}


double Sorcerer::GetRCTHeal() const {
    switch (GetRCTProficiency()) {
    case RCTProficiency::Crude: return 20.0;
    case RCTProficiency::Adept: return 35.0;
    case RCTProficiency::Expert: return 50.0;
    case RCTProficiency::Absolute: return 65.0;
    default: return 0.0;
    }
}
double Sorcerer::GetRCTCost() const {
    switch (GetRCTProficiency()) {
    case RCTProficiency::Crude: return 100.0;
    case RCTProficiency::Adept: return 75.0;
    case RCTProficiency::Expert: return 50.0;
    case RCTProficiency::Absolute: return 25.0;
    default: return 0.0;
    }
}
Sorcerer::RCTProficiency Sorcerer::GetRCTProficiency() const {
    return rct_skill;
}

void Sorcerer::UseRCT() {
    if (this->GetCharacterHealth() >= this->GetCharacterMaxHealth()) {
        return;
    }
    if (rct_state == ReverseCT::Active) {
        this->Regen(GetRCTHeal());
        this->SpendCEdirect(GetRCTCost());
    }
    else if (rct_state == ReverseCT::Overdrive) {
        this->Regen(GetRCTHeal() * 2);
        this->SpendCEdirect(GetRCTCost() * 2 );
    }
}

void Sorcerer::Attack(Character* target) {
    CurseUser* target_cuser = dynamic_cast<CurseUser*>(target);
    if (target_cuser) {
        if (Limitless* limitless = dynamic_cast<Limitless*>(target_cuser->GetTechnique())) {
            if (limitless->CheckInfinity() && !this->DomainAmplificationActive()) {
                std::println("{}'s attack was blocked by {}'s {}Infinity{}!", this->GetNameWithID(), target_cuser->GetNameWithID(),Color::Cyan,Color::Clear);
                return;
            }
        }
    }
    if (domain_amplification_active) {
        double ce_addon = std::sqrt(std::max(0.0, this->GetCharacterCE())) * 0.888;
        double amp_damage = base_attack_damage + ce_addon;

        target->DamageBypass(amp_damage);
        std::println("{} landed a strike on {} using {}domain amplification{}!", this->GetNameWithID(), target->GetNameWithID(),Color::Yellow,Color::Clear);
        return;
    }
    else if (cursed_tool) {
        cursed_tool->UseTool(this, target);
        return;
    }


    bool is_black_flash = false;
    if (GetRandomNumber(1, 100) <= black_flash_chance) {
        is_black_flash = true;
        if (this->technique) {
            technique->Set(Technique::Status::DomainBoost);
        }
    }

    double final_damage = base_attack_damage * (is_black_flash ? black_flash_multiplier : 1.0);
    target->Damage(final_damage);

    if (is_black_flash) {
        std::println("\n*** {}BLACK FLASH!{} ***", Color::Red, Color::Clear);
        std::println("{} landed a {}BlackFlash{} on {}!", this->GetNameWithID(), Color::Red, Color::Clear, target->GetNameWithID());
    }
    else {
        std::println("{} landed a {}heavy strike{} on {}!", this->GetNameWithID(),Color::BrightRed,Color::Clear, target->GetNameWithID());
    }
}

std::string Sorcerer::GetName() const {
    return "Sorcerer";
}

bool Sorcerer::CanBeHit() const {
    return true;
}

bool Sorcerer::IsaSorcerer()const {
    return true;
}

std::string Sorcerer::GetSimpleName() const {
    return "Sorcerer";
}

std::unique_ptr<Character> Sorcerer::Clone() const {
    return nullptr;
}