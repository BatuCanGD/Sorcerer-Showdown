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

double Technique::GetTechniqueOutput() const {
    switch (state) {
    case Status::Usable:      return 1.0;
    case Status::DomainBoost: return 1.5;
    case Status::BurntOut:    return 0.0;
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

void Limitless::InfinityNerf() {
	// do later
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
    println("{} uses Slash on {}!", user->GetName(), target->GetName());
    double dmg = CalculateDamage(user, slash_output);
    target->Damage(dmg);
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
    default:
        std::println("Invalid input. No technique used.");
    }
}

std::string Shrine::GetTechniqueName() const {
    return "Shrine";
}
