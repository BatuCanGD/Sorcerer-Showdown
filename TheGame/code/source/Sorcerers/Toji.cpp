#include "Sorcerer.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Domain.h"
#include "CursedTool.h"
#include "Utils.h"


import std;

// ---------------------- Toji ------------------


Toji::Toji() : Sorcerer(1300.0, -1, -1) {
    inventory_curse.push_back(std::make_unique<InvertedSpearofHeaven>());
    inventory_curse.push_back(std::make_unique<PlayfulCloud>());
    black_flash_chance = 0;
}

std::string Toji::GetName() const {
    return "Toji Fushiguro";
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

    if (!ignored) return;

    auto* limitless = dynamic_cast<Limitless*>(ignored->GetTechnique());

    if (limitless && limitless->CheckInfinity()) {
        if (!this->GetTool() || this->GetTool()->GetName() != "The Inverted Spear of Heaven") {
            this->EquipToolByName("The Inverted Spear of Heaven");
            return;
        }
    }
    else {
        if (!this->GetTool() || this->GetTool()->GetName() != "Playful Cloud") {
            this->EquipToolByName("Playful Cloud");
            return;
        }
    }

    this->GetTool()->UseTool(this, ignored);
}

bool Toji::CanBeHit() const {
    return true;
}