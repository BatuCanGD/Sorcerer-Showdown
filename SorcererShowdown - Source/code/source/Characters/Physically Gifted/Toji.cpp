#include "Toji.h"
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

void Toji::OnCharacterTurn(Character*, std::vector<std::unique_ptr<Character>>& battlefield) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }

    Character* target = nullptr;
    bool limitless_found = false;

    for (const auto& t : battlefield) {
        if (t.get() == this || t->GetCharacterHealth() <= 0.0) continue;

        auto* sorcerer = dynamic_cast<Sorcerer*>(t.get());
        bool has_limitless = sorcerer &&
            sorcerer->GetTechnique() &&
            sorcerer->GetTechnique()->GetTechniqueSimpleName() == "Limitless";

        if (!target) {
            target = t.get();
            limitless_found = has_limitless;
        }
        else if (has_limitless && !limitless_found) {
            target = t.get();
            limitless_found = true;
        }
        else if (!limitless_found &&
            t->GetCharacterHealth() == t->GetCharacterPreviousHealth()) {
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

    auto* sorcerer_target = dynamic_cast<Sorcerer*>(target);
    auto* limitless = sorcerer_target ? dynamic_cast<Limitless*>(sorcerer_target->GetTechnique()) : nullptr;

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