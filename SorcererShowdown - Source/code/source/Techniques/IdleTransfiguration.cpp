#include "IdleTransfiguration.h"
#include "TransfiguredHuman.h"
#include "BattlefieldHeader.h"
#include "Character.h"
#include "CurseUser.h"
#include "Utils.h"

import std;

std::string IdleTransfiguration::GetTechniqueName() const {
	return "\033[35mIdle Transfiguration\033[0m";
}
std::string IdleTransfiguration::GetTechniqueSimpleName() const {
	return "Idle Transfiguration";
}

void IdleTransfiguration::UseIdleTransfiguration(CurseUser* user, Character* target, TransfigurationType tf, Battlefield& bf) {
    switch (tf) {
    case TransfigurationType::Transfigure:
        UseTransfiguration(user, target);
        break;
    case TransfigurationType::Summon:
        SummonTransfiguredHumans(bf);
        break;
    default:
        std::println("Invalid Input");
    }
}

void IdleTransfiguration::UseTransfiguration(CurseUser* user, Character* target) {
    println("{} tried to transfigure {}!", user->GetNameWithID(), target->GetNameWithID());
    double dmg = CalculateDamage(user, transfiguration_damage);
    target->Damage(dmg);
    if (target->GetCharacterHealth() <= 0.0) {
        transfigured_human_count++;
        std::println("{} has gained a transfigured human body", user->GetNameWithID());
    }
    chant = ChantLevel::Zero;
}

void IdleTransfiguration::SummonTransfiguredHumans(Battlefield& bf) {
    if (transfigured_human_count > 0) {
        bf.spawn_queue.push_back(std::make_unique<TransfiguredHuman>());
        transfigured_human_count--;
    }
}

void IdleTransfiguration::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (user->DomainAmplificationActive()) {
        std::println("{}You cannot use your innate technique due to domain amplification!{}", Color::Red, Color::Clear);
        return;
    }
    std::println("1 - Use Idle Transfiguration | 2 - Summon Transfigured Humans");
    

    std::print("=> ");
    size_t choice = GetValidInput();
    switch (choice) {
    case 1:
        UseTransfiguration(user, target);
        break;
    case 2:
        SummonTransfiguredHumans(bf);
        break;
    default:
        std::println("Invalid Choice");
        break;
    }
}

int IdleTransfiguration::GetTFcount() const {
    return transfigured_human_count;
}

void IdleTransfiguration::Chant() {}

void IdleTransfiguration::TechniqueSetting(CurseUser*, Battlefield&) {
    std::println("===== Idle Transfiguration Menu =====");
    std::println("Transfigured human amount in storage: {}", transfigured_human_count);

}

std::unique_ptr<Technique> IdleTransfiguration::Clone() const {
    return std::make_unique<IdleTransfiguration>();
}