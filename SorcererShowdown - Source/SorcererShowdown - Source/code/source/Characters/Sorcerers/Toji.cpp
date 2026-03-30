#include "Toji.h"
#include "Limitless.h"
#include "CursedToolList.h"
#include "Utils.h"


import std;

// ---------------------- Toji ------------------


Toji::Toji() : Sorcerer(1300.0, -1, -1) {
    inventory_curse.push_back(std::make_unique<InvertedSpearofHeaven>());
    inventory_curse.push_back(std::make_unique<PlayfulCloud>());
    black_flash_chance = 0;
}

std::unique_ptr<Sorcerer> Toji::Clone() const {
    return std::make_unique<Toji>();
}

std::string Toji::GetName() const {
    return "\033[33mToji Fushiguro\033[0m";
}

void Toji::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }

    Sorcerer* ignored = nullptr;
    for (const auto& t : battlefield) {
        if (t.get() == this || t->GetCharacterHealth() <= 0.0) continue;

        if (!ignored || t->GetCharacterHealth() == t->GetCharacterPreviousHealth() || 
            (t->GetTechnique() && t->GetTechnique()->GetTechniqueName() == "Limitless")) 
        {
            ignored = t.get();
        }
    }

    if (!ignored) {
        std::println("Cant find a proper sorcerer to fight these days");
        return;
    }

    int tntroll = GetRandomNumber(1, 20);
    if (tntroll <= 10) {
        this->Taunt(ignored);
    }

    auto* limitless = dynamic_cast<Limitless*>(ignored->GetTechnique());

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
    if (cursed_tool) {
        this->GetTool()->UseTool(this, ignored);
    }
    else {
        this->Attack(ignored);
    }
}

bool Toji::CanBeHit() const {
    return true;
}