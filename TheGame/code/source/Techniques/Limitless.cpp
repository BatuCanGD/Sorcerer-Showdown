#include "Techniques.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

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
    if (this->BurntOut()) {
        if (CheckInfinity()) {
            std::println("{}'s Infinity shatters due to technique burnout!", user->GetName());
            SetInfinity(false);
        }
        return;
    }
    if (this->CheckInfinity()) {
        double maintain_cost = 100.0;
        if (user->GetCharacterCE() < maintain_cost) {
            std::println("{}'s concentration wavers due to low CE! Infinity is deactivated.", user->GetName());
            SetInfinity(false);
        }
        else {
            user->SpendCEdirect(maintain_cost);
        }
    }
}

void Limitless::TechniqueMenu(Sorcerer* user, Character* target) {
    if (user->DomainAmplificationActive()) {
        std::println("You cannot use your innate technique due to domain amplification!");
        return;
    }
    std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple");
    std::print("=> ");
    size_t choice = GetValidInput();
    UseTheLimitlessTechnique(static_cast<LimitlessType>(choice), user, target);
}

void Limitless::TechniqueSetting(Sorcerer* user, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
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

std::unique_ptr<Technique> Limitless::Clone() const {
    return std::make_unique<Limitless>(*this);
}
