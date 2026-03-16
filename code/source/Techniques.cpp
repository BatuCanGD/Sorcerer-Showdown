#include "Technique.h"
#include "Sorcerer.h"
#include "Character.h"

import std;

void Technique::Set(Status s) {
    state = s;
}

Technique::Status Technique::GetStatus() const {
    return state;
}

std::string Technique::GetTechniqueName() const {
    return "Technique";
}

double Technique::GetTechniqueOutput() const {
    switch (state) {
    case Status::Usable:      return 1.0;
    case Status::DomainBoost: return 2.0;
    case Status::BurntOut:    return 0.35;
    default:                  return 1.0;
    }
}

std::string Technique::GetStringStatus() const {
    switch (state) {
	case Status::Usable:      return "Usable";
	case Status::DomainBoost: return "Output Boosted";
	case Status::BurntOut:    return "Burnt Out";
    default:                  return "Unknown State";
    }
}

bool Technique::Boosted() const {
    return state == Status::DomainBoost;
}
bool Technique::Usable() const {
    return state == Status::Usable;
}
bool Technique::BurntOut() const {
    return state == Status::BurntOut;
}

double Technique::CalculateDamage(Sorcerer* user, double cost) {
    double multiplier = GetTechniqueOutput();
    double currentCE = user->GetCharacterCE();

    if (currentCE < cost) {
        std::println("Insufficient Cursed Energy! Output weakened.");
        user->SpendCE(currentCE); 
        return currentCE * multiplier;
    }
    user->SpendCE(cost);
    return cost * multiplier;
}
// ---------------- Limitless -------------------




double Limitless::BlueTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Blue on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, blue_output);
    target->Damage(dmg);
    return dmg;
}

double Limitless::RedTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Red on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, red_output);
    target->Damage(dmg);
    return dmg;
}

double Limitless::PurpleTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} hits {} with a Hollow Purple!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, purple_output);
    target->Damage(dmg);
    return dmg;
}

void Limitless::UseTheLimitlessTechnique(LimitlessType choice, Sorcerer* s, Character* c) {
    switch (choice) {
    case LimitlessType::Blue:
		Limitless::BlueTechniqueDamageTarget(s, c);
        break;
    case LimitlessType::Red:
		Limitless::RedTechniqueDamageTarget(s, c);
        break;
    case LimitlessType::Purple:
		Limitless::PurpleTechniqueDamageTarget(s, c);
        break;
    default:
        std::println("Invalid input. No technique used.");
    }
}

std::string Limitless::GetTechniqueName() const {
    return "Limitless";
}

void Limitless::SetInfinity(bool s) {
    Infinity = s;
}

bool Limitless::CheckInfinity() const {
    return Infinity;
}

void Limitless::InfinityNerf(Sorcerer* user) {
    if (Infinity) {
        double maintain_cost = 100.0;
        if (user->GetCharacterCE() < maintain_cost) {
            std::println("{}'s concentration wavers due to low CE! Infinity is deactivated.", user->GetName());
            SetInfinity(false);
        }
        else {
            user->SetCursedEnergy(user->GetCharacterCE() - 50.0);
        }
    }
}

void Limitless::TechniqueMenu(Sorcerer* user, Character* target) {
    std::println("1-use Blue, 2-use Red, 3-use Purple");
    std::print("=> ");
    size_t choice = 0;
    std::cin >> choice;
    UseTheLimitlessTechnique(static_cast<LimitlessType>(choice), user, target);
}

// ---------------- Shrine -------------------

void Shrine::SetWCS(bool s) {
    world_cutting_slash_allowed = s;
}

double Shrine::CleaveTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Cleave on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, cleave_output);
    target->Damage(dmg);
    return dmg;
}
double Shrine::DismantleTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Dismantle on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, slash_output);
    target->Damage(dmg);
    return dmg;
}
double Shrine::WorldCuttingSlashToTarget(Sorcerer* user, Character* target) {
    println("{} uses the World Cutting Slash on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, wcs_output);
    target->DamageBypass(dmg);
    return dmg;
}

void Shrine::UseShrineTechnique(ShrineType choice, Sorcerer* s, Character* c) {
    switch (choice) {
    case ShrineType::Dismantle:
        Shrine::DismantleTechniqueDamageTarget(s, c);
        break;
    case ShrineType::Cleave:
        Shrine::CleaveTechniqueDamageTarget(s, c);
        break;
    case ShrineType::WCS:
        if (!world_cutting_slash_allowed) return;
        Shrine::WorldCuttingSlashToTarget(s, c);
        break;
    default:
        std::println("Invalid input. No technique used.");
    }
}

std::string Shrine::GetTechniqueName() const {
    return "Shrine";
}

bool Shrine::WorldCuttingSlashUnlocked() const {
    return world_cutting_slash_allowed;
}

void Shrine::TechniqueMenu(Sorcerer* user, Character* target) {
    if (world_cutting_slash_allowed) {
        std::println("1-use Dismantle, 2-use Cleave, 3-use WCS");
    }
    else {
        std::println("1-use Dismantle, 2-use Cleave");
    }

    std::print("=> ");
    size_t choice = 0;
    std::cin >> choice;
    UseShrineTechnique(static_cast<ShrineType>(choice), user, target);
}
