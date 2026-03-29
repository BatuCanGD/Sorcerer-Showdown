#include "Limitless.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

void Limitless::BlueTechniqueDamageTarget(Sorcerer* user, Character* target) {
    if (chant == ChantLevel::Four) std::println("{}\"MAXIMUM OUTPUT: BLUE!\"{}", Color::Blue, Color::Clear);
    println("{} uses {}Blue{} on {}!", user->GetName(),Color::Blue,Color::Clear, target->GetName());
    double dmg = CalculateDamage(user, blue_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}

void Limitless::RedTechniqueDamageTarget(Sorcerer* user, Character* target) {
    if (chant == ChantLevel::Four) std::println("{}\"MAXIMUM OUTPUT: RED!\"{}",Color::Red, Color::Clear);
    println("{} uses {}Red{} on {}!", user->GetName(),Color::Red,Color::Clear, target->GetName());
    double dmg = CalculateDamage(user, red_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}

void Limitless::PurpleTechniqueDamageTarget(Sorcerer* user, Character* target) {
    if (chant == ChantLevel::Four) std::println("{}\"HOLLOW PURPLE!\"{}",Color::Purple,Color::Clear);
    println("{} hits {} with a {}Hollow Purple!{}", user->GetName(), target->GetName(), Color::Purple,Color::Clear);
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
    return "\033[36mLimitless\033[0m";
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
            std::println("{}{}'s Infinity shatters due to technique burnout!{}",Color::Cyan, user->GetName(), Color::Clear);
            SetInfinity(false);
        }
        return;
    }
    if (this->CheckInfinity()) {
        double maintain_cost = 100.0;
        if (user->GetCharacterCE() < maintain_cost) {
            std::println("{}{}'s concentration wavers due to low CE!{}{} Infinity is deactivated.{}",Color::Red,user->GetName(),Color::Clear,Color::Cyan,Color::Clear);
            SetInfinity(false);
        }
        else {
            user->SpendCEdirect(maintain_cost);
        }
    }
}

void Limitless::TechniqueMenu(Sorcerer* user, Character* target) {
    if (user->DomainAmplificationActive()) {
        std::println("{}You cannot use your innate technique due to domain amplification!{}", Color::Red, Color::Clear);
        return;
    }
    std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple");
    std::print("=> ");
    size_t choice = GetValidInput();
    UseTheLimitlessTechnique(static_cast<LimitlessType>(choice), user, target);
}

void Limitless::TechniqueSetting(Sorcerer* user, const std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    std::println("Infinity Status: [{}] | Chant level: [{}]", this->CheckInfinity() ? "\033[36mActive\033[0m" : "\033[31mInactive\033[0m", this->GetStringChantLevel());
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
        std::println("\"{}Phase. Twilight.{}\"",Color::Cyan,Color::Clear);
        chant = ChantLevel::One;
        return;
    }
    else if (chant == ChantLevel::One) {
        std::println("\"{}Paramita. Pillars of Light.{}\"",Color::Blue,Color::Clear);
        chant = ChantLevel::Two;
        return;
    }
    else if (chant == ChantLevel::Two) {
        std::println("\"{}Nine ropes. Polarized light. Crow and Shomyo.{}\"",Color::BrightRed,Color::Clear);
        chant = ChantLevel::Three;
        return;
    }
    else if (chant == ChantLevel::Three) {
        std::println("\"{}The gap between within and without.{}\"",Color::BrightMagenta,Color::Clear);
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
