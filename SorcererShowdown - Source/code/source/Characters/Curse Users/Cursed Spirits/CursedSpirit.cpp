#include "CursedSpirit.h"
#include "CursedTool.h"
#include "Limitless.h"
#include "Utils.h"

import std;

CursedSpirit::CursedSpirit(double hp, double ce, double regen) : CurseUser(hp, ce, regen) {};

std::string CursedSpirit::GetSimpleName() const {
	return "Cursed Spirit";
}
std::string CursedSpirit::GetName() const {
	return "\033[38;5;183mCursed Spirit\033[0m";
}
bool CursedSpirit::CanBeHit() const {
	return true;
}

bool CursedSpirit::IsaCursedSpirit() const {
    return true;
}

void CursedSpirit::Attack(Character* target) {
    CurseUser* target_cuser = dynamic_cast<CurseUser*>(target);
    if (target_cuser) {
        if (Limitless* limitless = dynamic_cast<Limitless*>(target_cuser->GetTechnique())) {
            if (limitless->CheckInfinity() && !this->DomainAmplificationActive()) {
                std::println("{}'s attack was blocked by {}'s {}Infinity{}!", this->GetNameWithID(), target_cuser->GetNameWithID(), Color::Cyan, Color::Clear);
                return;
            }
        }
    }
    if (domain_amplification_active) {
        double ce_addon = std::sqrt(std::max(0.0, this->GetCharacterCE())) * 0.888;
        double amp_damage = base_attack_damage + ce_addon;

        target->DamageBypass(amp_damage);
        std::println("{} landed a strike on {} using {}domain amplification{}!", this->GetNameWithID(), target->GetNameWithID(), Color::Yellow, Color::Clear);
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
        std::println("{} landed a {}heavy strike{} on {}!", this->GetNameWithID(), Color::BrightRed, Color::Clear, target->GetNameWithID());
    }
}