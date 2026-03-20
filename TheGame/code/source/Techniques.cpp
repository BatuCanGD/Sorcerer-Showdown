#include "Techniques.h"
#include "Sorcerer.h"
#include "Character.h"
#include "Utils.h"

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

double Technique::GetChantPower()const {
    return 1.0 + (static_cast<int>(this->chant) * 0.50);
}

std::string Technique::GetStringChantLevel() const {
    switch (chant) {
    case ChantLevel::Zero: return "Zero";
    case ChantLevel::One: return "One";
    case ChantLevel::Two: return "Two";
    case ChantLevel::Three: return "Three";
    case ChantLevel::Four: return "Four";
    default: return "One";
    }
}

// ---------------- Limitless -------------------




void Limitless::BlueTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Blue on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, blue_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}

void Limitless::RedTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Red on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, red_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}

void Limitless::PurpleTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} hits {} with a Hollow Purple!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, purple_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
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
    std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple");
    std::print("=> ");
    size_t choice = GetValidInput();
    UseTheLimitlessTechnique(static_cast<LimitlessType>(choice), user, target);
}

void Limitless::TechniqueSetting(Sorcerer* user) {
    std::println("Infinity Status: [{}] | Chant level: [{}]", this->CheckInfinity() ? "Active" : "Inactive", this->GetStringChantLevel());
    std::println("1 - Turn on Infinity | 2 - Turn off Infinity | 3 - Chant | 4 - Return");
    std::print("=> ");
    int ch = GetValidInput();
    switch (ch) {
    case 1:
        if (user->GetCharacterCE() < 100.0) {
            std::println("You do not have enough Cursed Energy to alter Infinity's state.");
            return;
        }
        else if (CheckInfinity()) {
            std::println("Infinity is already active");
            return;
        }
        this->SetInfinity(true);
        std::println("\nInfinity has been Activated");
        break;
    case 2:
        if (!CheckInfinity()) {
            std::println("Infinity is already Disabled");
            return;
        }
        this->SetInfinity(false);
        std::println("\nInfinity has been Deactivated");
        break;
    case 3:
        this->Chant();
        break;
    case 4:
        break;
    default:
        std::println("Invalid Input! Skipping turn");
        }

    
}

void Limitless::Chant() {
    if (chant == ChantLevel::Zero) {
        std::println("\"Phase. Twilight.\"");
        chant = ChantLevel::One;
        return;
    }
    else if (chant == ChantLevel::One) {
        std::println("\"Paramita. Pillars of Light.\"");
        chant = ChantLevel::Two;
        return;
    }
    else if (chant == ChantLevel::Two) {
        std::println("\"Nine ropes. Polarized light. Crow and Shomyo.\"");
        chant = ChantLevel::Three;
        return;
    }
    else if (chant == ChantLevel::Three) {
        std::println("\"The gap between within and without.\"");
        chant = ChantLevel::Four;
        return;
    }
    else {
        std::println("Its time to use your Technique, its not gonna get anymore stronger");
    }
}

// ---------------- Shrine -------------------

void Shrine::SetWCS(bool s) {
    world_cutting_slash_allowed = s;
}

void Shrine::CleaveTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Cleave on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, cleave_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}
void Shrine::DismantleTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Dismantle on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, slash_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}
void Shrine::WorldCuttingSlashToTarget(Sorcerer* user, Character* target) {
    if (this->chant != ChantLevel::Four) {
        std::println("{} hasn't completed the incantations! The slash fails to divide the world...", user->GetName());
        return;
    }
    println("{} uses the World Cutting Slash on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, wcs_output * GetChantPower());
    target->DamageBypass(dmg);
    chant = ChantLevel::Zero;
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
        std::println("1 - Use Dismantle | 2 - Use Cleave || 3 - Use the World Cutting Slash");
    }
    else {
        std::println("1 - Use Dismantle | 2 - Use Cleave");
    }

    std::print("=> ");
    size_t choice = GetValidInput();
    UseShrineTechnique(static_cast<ShrineType>(choice), user, target);
}

void Shrine::TechniqueSetting(Sorcerer* user) {
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
        std::println("\"Scale of the Dragon\"");
        chant = ChantLevel::One;
    }
    else if (chant == ChantLevel::One) {
        std::println("\"Recoil.\"");
        chant = ChantLevel::Two;
    }
    else if (chant == ChantLevel::Two) {
        std::println("\"Twin Meteors.\"");
        chant = ChantLevel::Three;
    }
    else if (chant == ChantLevel::Three) {
        std::println("\"Silence. Desolation. Anatomy of Ruin.\"");
        chant = ChantLevel::Four;
    }
    else {
        std::println("The technique is already at maximum output. Sever them!");
    }
}