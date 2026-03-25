#include "Sorcerer.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Domain.h"
#include "CursedTool.h"
#include "Utils.h"

import std;

// -------------- Yuta ------------------

Yuta::Yuta() : Sorcerer(800.0, 6000.0, 25.0) {
    technique = std::make_unique<Copy>();
    cursed_tool = std::make_unique<Katana>();
    domain = std::make_unique<AuthenticMutualLove>();
    counter_domain = std::make_unique<SimpleDomain>();
    shikigami.push_back(std::make_unique<Rika>());
    black_flash_chance = 10;
}

std::string Yuta::GetName() const {
    return "Yuta Okkotsu";
}

void Yuta::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }
    if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.65) {
        this->BoostRCT();
    }
    else if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.95) {
        this->EnableRCT();
    }
    else {
        this->DisableRCT();
    }
    double strongesthealth = -1.0;
    Sorcerer* strongest = nullptr;
    std::vector<Sorcerer*> domain_users;

    auto rika = this->ChooseShikigami(0);

    if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.40 && \
        !(rika->GetActiveTime() > 5) && !rika->IsActivePhysically()){

        std::println("Come, Rika.");
        rika->Manifest();
    }

    for (const auto& s : battlefield) {
        if (s.get() == this) continue;

        if (s->DomainActive()) {
            domain_users.push_back(s.get());
        }

        if (s->GetCharacterHealth() > strongesthealth) {
            strongesthealth = s->GetCharacterHealth();
            strongest = s.get();
        }
    }

    if (domain_users.size() > 0) {
        if (domain_users.size() >= 2 || this->GetTechnique()->BurntOut() || this->GetDomainUses() >= 5) {
            this->ActivateCounterDomain();
            return;
        }
        else if (!this->DomainActive() && !this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5) {
            this->ActivateDomain();
            return;
        }
    }

    if (strongest) {
        auto limitless = dynamic_cast<Limitless*>(strongest->GetTechnique());

        if (limitless && limitless->CheckInfinity()) {
            this->SetAmplification(true);
        }
        else {
            this->SetAmplification(false);
        }
        this->GetTool()->UseTool(this, strongest);
    }
}

bool Yuta::CanBeHit() const {
    return true;
}
