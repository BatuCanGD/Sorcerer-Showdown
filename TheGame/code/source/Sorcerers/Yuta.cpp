#include "Sorcerer.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Domain.h"
#include "CursedTool.h"
#include "Utils.h"

import std;

// -------------- Yuta ------------------

Yuta::Yuta() : Sorcerer(800.0, 6000.0, 30.0) {
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
    Shikigami* rika = this->ChooseShikigami(0);

    if (!this->HPMoreThanMax(0.40) || rika->IsActivePhysically()) {
        this->BoostRCT();
    }
    else if (!(this->HPMoreThanMax(0.70))) {
        this->EnableRCT();
    }
    else {
        this->DisableRCT();
    }

    double strongesthealth = -1.0;
    Sorcerer* strongest = nullptr;
    std::vector<Sorcerer*> domain_users;

    if (!this->HPMoreThanMax(0.40) && !(rika->GetActiveTime() > 5) && !rika->IsActivePhysically()){

        std::println("Come, Rika.");
        rika->Manifest();
    }

    for (const auto& s : battlefield) {
        if (s.get() == this) continue;

        if (s->DomainActive()) {
            domain_users.push_back(s.get());
        }
        double perceived_health = s->GetCharacterHealth() + GetRandomNumber(-100, 100);
        if (!strongest || perceived_health > strongesthealth) {
            strongesthealth = perceived_health;
            strongest = s.get();
        }
    }

    if (domain_users.size() > 0) {
        if (domain_users.size() == 1 && !this->CounterDomainActive() && (this->GetTechnique()->BurntOut() || this->GetDomainUses() >= 5)) {
            this->ActivateCounterDomain();
            return;
        }
        else if (domain_users.size() == 1 && !this->DomainActive() && !this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5) {
            this->ActivateDomain();
            return;
        }
    }
    else {
        if (this->CounterDomainActive()) this->DeactivateCounterDomain();
    }

    if (strongest) {
        auto limitless = dynamic_cast<Limitless*>(strongest->GetTechnique());

        if (limitless && limitless->CheckInfinity()) {
            this->SetAmplification(true);
        }
        else {
            this->SetAmplification(false);
        }
        this->Attack(strongest);
    }
}

bool Yuta::CanBeHit() const {
    return true;
}
