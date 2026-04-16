#include "Toji.h"
#include "BattlefieldHeader.h"
#include "Limitless.h"
#include "CursedToolList.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

Toji::Toji() : PhysicallyGifted(1000.0, 450.0) {
    inventory_curse.push_back(std::make_unique<InvertedSpearofHeaven>());
    inventory_curse.push_back(std::make_unique<PlayfulCloud>());
}

std::unique_ptr<Character> Toji::Clone() const {
    return std::make_unique<Toji>();
}

std::string Toji::GetName() const {
    return "\033[33mToji Fushiguro\033[0m";
}
std::string Toji::GetSimpleName() const {
    return "Toji Fushiguro";
}

void Toji::TickCharacterSpecialty() {
    this->Regen(this->GetHealingFactor());
}

void Toji::OnCharacterTurn(Character*, Battlefield& bf) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }

    Character* target = nullptr;
    double best_score = -1.0;

    for (const auto& t : bf.battlefield) {
        if (t.get() == this || t->GetCharacterHealth() <= 0.0) continue;

        auto* curse_user = dynamic_cast<CurseUser*>(t.get());
        
        double hp_ratio = t->GetCharacterHealth() / t->GetCharacterMaxHealth();
        double score = 1.0 - hp_ratio;

        if (curse_user && curse_user->GetTechnique()) {
            auto* limitless = dynamic_cast<Limitless*>(curse_user->GetTechnique());
            if (limitless && limitless->CheckInfinity()) {
                score += 0.15;
            }
        }
        score += GetRandomNumber(-5, 5) * 0.01;
        if (score > best_score) {
            best_score = score;
            target = t.get();
        }
    }

    if (!target) {
        std::println("Can't find a proper sorcerer to fight these days...");
        return;
    }

    int tntroll = GetRandomNumber(1, 20);
    if (tntroll <= 10) {
        this->Taunt(target);
    }

    auto* curse_user_t = dynamic_cast<CurseUser*>(target);
    auto* limitless = curse_user_t ? dynamic_cast<Limitless*>(curse_user_t->GetTechnique()) : nullptr;

    if (limitless && limitless->CheckInfinity()) {
        if (!this->GetTool() || this->GetTool()->GetSimpleName() != "The Inverted Spear of Heaven") {
            this->EquipToolByName("The Inverted Spear of Heaven");
            return;
        }
    }
    else {
        if (!this->GetTool() || this->GetTool()->GetSimpleName() != "Playful Cloud") {
            this->EquipToolByName("Playful Cloud");
            return;
        }
    }

    if (this->GetTool()) {
        this->GetTool()->UseTool(this, target);
    }
    else {
        std::println("{} strikes {} with his bare hands!", this->GetNameWithID(), target->GetNameWithID());
        target->Damage(10.0 * this->GetStrengthDamage());
    }
}

bool Toji::CanBeHit() const {
    return true;
}