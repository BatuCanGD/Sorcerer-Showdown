#include "Technique.h"
#include "Sorcerer.h"
#include "Character.h"

void Technique::Set(Status s) {
    state = s;
}

Technique::Status Technique::GetStatus() const {
    return state;
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

double Limitless::CalculateDamage(Sorcerer* user, double cost) const {
    double multiplier = GetTechniqueOutput();
    double currentCE = user->GetCharacterCE();

    if (currentCE < cost) {
        println("Insufficient Cursed Energy! Output weakened.");
        user->SpendCE(currentCE); 
        return currentCE * multiplier;
    }
    user->SpendCE(cost);
    return cost * multiplier;
}

double Limitless::BlueTechniqueDamageTarget(Sorcerer* user, Character* target) const {
    println("{} uses Blue!", user->GetName());
    double dmg = CalculateDamage(user, blue_output);
    target->Damage(dmg);
    return dmg;
}

double Limitless::RedTechniqueDamageTarget(Sorcerer* user, Character* target) const {
    println("{} uses Red!", user->GetName());
    double dmg = CalculateDamage(user, red_output);
    target->Damage(dmg);
    return dmg;
}

double Limitless::PurpleTechniqueDamageTarget(Sorcerer* user, Character* target) const {
    println("{} hollows out the battlefield with Purple!", user->GetName());
    double dmg = CalculateDamage(user, purple_output);
    target->Damage(dmg);
    return dmg;
}

// ---------------- Shrine -------------------

void Shrine::SetWCS(bool s) {
    world_cutting_slash_allowed = s;
}