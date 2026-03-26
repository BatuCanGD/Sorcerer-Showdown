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

double Technique::CalculateDamage(Sorcerer* user, double cost) const {
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
    case ChantLevel::Four: return "Max";
    default: return "One";
    }
}