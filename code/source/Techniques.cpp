#include "Technique.h"
#include "Sorcerer.h"
#include "Character.h"
#include <print>

void Technique::Set(Status s) {
    state = s;
}

Technique::Status Technique::GetStatus() const {
    return state;
}

std::string Technique::GetTechniqueName() const {
    return "Technique";
}

std::string Technique::GetStringStatus() const {
    switch (state) {
	case Status::Usable:      return "Usable";
	case Status::DomainBoost: return "Output Boosted";
	case Status::BurntOut:    return "Burnt Out";
    default:                  return "Unknown State";
    }
}

// ---------------- Limitless -------------------

double Limitless::GetTechniqueOutput() const {
    switch (state) {
    case Status::Usable:      return 1.0;
    case Status::DomainBoost: return 2.5;
    case Status::BurntOut:    return 0.0;
    default:                  return 1.0;
    }
}

double Limitless::CalculateDamage(Sorcerer* user, double cost) {
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

double Limitless::BlueTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Blue!", user->GetName());
    double dmg = CalculateDamage(user, blue_output);
    target->Damage(dmg);
    return dmg;
}

double Limitless::RedTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} uses Red!", user->GetName());
    double dmg = CalculateDamage(user, red_output);
    target->Damage(dmg);
    return dmg;
}

double Limitless::PurpleTechniqueDamageTarget(Sorcerer* user, Character* target) {
    println("{} hollows out the battlefield with Purple!", user->GetName());
    double dmg = CalculateDamage(user, purple_output);
    target->Damage(dmg);
    return dmg;
}

void Limitless::UseTheLimitlessTechnique(int choice, Sorcerer* s, Character* c) {
    switch (choice) {
    case 1:
		Limitless::BlueTechniqueDamageTarget(s, c);
        break;
    case 2:
		Limitless::RedTechniqueDamageTarget(s, c);
        break;
    case 3:
		Limitless::PurpleTechniqueDamageTarget(s, c);
        break;
    default:
        std::println("Invalid input. No technique used.");
    }
}

std::string Limitless::GetTechniqueName() const {
    return "Limitless";
}


// ---------------- Shrine -------------------

void Shrine::SetWCS(bool s) {
    world_cutting_slash_allowed = s;
}

std::string Shrine::GetTechniqueName() const {
    return "Shrine";
}