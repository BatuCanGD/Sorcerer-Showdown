#include "Shrine.h"
#include "CurseUser.h"
#include "Character.h"
#include "Utils.h"

import std;

void Shrine::SetWCS(bool s) {
    world_cutting_slash_allowed = s;
}

void Shrine::UseCleave(CurseUser* user, Character* target) {
    println("{} uses {}Cleave{} on {}!", user->GetNameWithID(),Color::Red,Color::Clear, target->GetNameWithID());
    double dmg = CalculateDamage(user, cleave_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}
void Shrine::UseDismantle(CurseUser* user, Character* target) {
    println("{} uses {}Dismantle{} on {}!", user->GetNameWithID(),Color::Red,Color::Clear, target->GetNameWithID());
    double dmg = CalculateDamage(user, slash_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}
void Shrine::UseTheWorldCuttingSlash(CurseUser* user, Character* target) {
    if (this->chant != ChantLevel::Four) {
        std::println("{} hasn't completed the incantations! {}The slash fails to divide the world...{}", user->GetName(), Color::Red,Color::Clear);
        return;
    }
    println("{} uses the {}World Cutting Slash{} on {}!", user->GetNameWithID(),Color::Red,Color::Clear, target->GetNameWithID());
    double dmg = CalculateDamage(user, wcs_output * GetChantPower());
    target->DamageBypass(dmg);
    chant = ChantLevel::Zero;
}

void Shrine::UseShrineTechnique(ShrineType choice, CurseUser* s, Character* c) {
    switch (choice) {
    case ShrineType::Dismantle:
        UseDismantle(s, c);
        break;
    case ShrineType::Cleave:
        UseCleave(s, c);
        break;
    case ShrineType::WCS:
        if (!world_cutting_slash_allowed) return;
        UseTheWorldCuttingSlash(s, c);
        break;
    default:
        std::println("Invalid input. No technique used.");
    }
}

std::string Shrine::GetTechniqueName() const {
    return "\033[31mShrine\033[0m";
}
std::string Shrine::GetTechniqueSimpleName() const {
    return "Shrine";
}

bool Shrine::WorldCuttingSlashUnlocked() const {
    return world_cutting_slash_allowed;
}

void Shrine::TechniqueMenu(CurseUser* user, Character* target, const std::vector<std::unique_ptr<Character>>& battlefield) {
    if (user->DomainAmplificationActive()) {
        std::println("You cannot use your innate technique due to domain amplification!");
        return;
    }
    if (world_cutting_slash_allowed) {
        std::println("1 - Use Dismantle | 2 - Use Cleave || 3 - {}Use the World Cutting Slash{}",Color::Red,Color::Clear);
    }
    else {
        std::println("1 - Use Dismantle | 2 - Use Cleave");
    }

    std::print("=> ");
    size_t choice = GetValidInput();
    UseShrineTechnique(static_cast<ShrineType>(choice), user, target);
}

void Shrine::TechniqueSetting(CurseUser* user, const std::vector<std::unique_ptr<Character>>& battlefield) {
    std::println("Chant level: [{}]", this->GetStringChantLevel());
    std::println("1 - Chant | 2 - Return");
    std::print("=> ");

    int ch = GetValidInput();
    switch (ch) {
    case 1:
        this->Chant();
        break;
    case 2:
        break;
    default:
        std::println("Invalid Input! Skipping turn");
    }
}

void Shrine::Chant() {
    if (chant == ChantLevel::Zero) {
        std::println("{}\"Scale of the Dragon\"{}",Color::BrightRed,Color::Clear);
        chant = ChantLevel::One;
    }
    else if (chant == ChantLevel::One) {
        std::println("{}\"Recoil.\"{}", Color::BrightRed, Color::Clear);
        chant = ChantLevel::Two;
    }
    else if (chant == ChantLevel::Two) {
        std::println("{}\"Twin Meteors.\"{}", Color::Red, Color::Clear);
        chant = ChantLevel::Three;
    }
    else if (chant == ChantLevel::Three) {
        std::println("{}\"Silence. Desolation. Anatomy of Ruin.\"{}", Color::Red, Color::Clear);
        chant = ChantLevel::Four;
    }
    else {
        std::println("The technique is already at maximum output. Sever them!");
    }
}

std::unique_ptr<Technique> Shrine::Clone() const {
    return std::make_unique<Shrine>(*this);
}