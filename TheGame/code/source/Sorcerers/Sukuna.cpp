#include "Sorcerer.h"
#include "Techniques.h"
#include "Shikigami.h"
#include "Specials.h"
#include "Domain.h"
#include "Utils.h"

import std;

Sukuna::Sukuna() : Sorcerer(1000.0, 16000.0, 150.0) {
    domain = std::make_unique<MalevolentShrine>();
    counter_domain = std::make_unique<HollowWickerBasket>();
    technique = std::make_unique<Shrine>();
    shikigami.push_back(std::make_unique<Mahoraga>());
    shikigami.push_back(std::make_unique<Agito>());
    special = std::make_unique<WorldCuttingSlash>();
    black_flash_chance = 10;
}

std::string Sukuna::GetName() const {
    return "Sukuna";
}

void Sukuna::OnSorcererTurn(std::vector<std::unique_ptr<Sorcerer>>& battlefield) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }
    if (!this->HPMoreThanMax(0.10) && this->CEMoreThanMax(0.25))
    {
        this->BoostRCT();
    }
    else if (!this->HPMoreThanMax(0.75) && this->CEMoreThanMax(0.10))
    {
        this->EnableRCT();
    }
    else
    {
        this->DisableRCT();
    }

    double weakesthealth = INT32_MAX;
    Sorcerer* weakest = nullptr;
    std::vector<Sorcerer*> domain_users;
    for (const auto& target : battlefield) {
        if (target.get() == this) continue;
        double perceived_health = target->GetCharacterHealth() + GetRandomNumber(-100, 100);
        if (target->DomainActive()) {
            domain_users.push_back(target.get());
        }
        if (perceived_health < weakesthealth || !weakest) {
            weakesthealth = perceived_health;
            weakest = target.get();
        }
    }

    Mahoraga* makora = nullptr;
    Agito* agito = nullptr;

    for (const auto& s : shikigami) {
        if (s->GetName() == "Mahoraga") {
            makora = dynamic_cast<Mahoraga*>(s.get());
        }
        else if (s->GetName() == "Agito") {
            agito = dynamic_cast<Agito*>(s.get());
        }
    }

    Shrine* shrine = dynamic_cast<Shrine*> (this->GetTechnique());

    if (makora) {
        if (!shrine->WorldCuttingSlashUnlocked()) {
            if (!makora->IsActivePhysically() && this->CEMoreThanMax(0.75)) {
                makora->Manifest();
            }
            else if (!makora->IsActive() && this->CEMoreThanMax(0.235)) {
                makora->PartiallyManifest();
            }
            else if (!this->CEMoreThanMax(0.235)) {
                makora->Withdraw();
            }
        }

        if (makora->FullyAdapted() && !shrine->WorldCuttingSlashUnlocked()) {
            this->GetSpecial()->PerformSpecial(this);
            makora->Withdraw();
            return;
        }
    }

    if (agito && !this->HPMoreThanMax(0.35)) {
        if (!this->CEMoreThanMax(0.30) && agito->IsActive()) {
            agito->Withdraw();
        }
        else if (!agito->IsActive() && this->HPMoreThanMax(0.50) && this->CEMoreThanMax(0.30)) {
            agito->Manifest();
        }
    }

    int roll = GetRandomNumber(1, 100);

    if (shrine->WorldCuttingSlashUnlocked() && this->CEMoreThanMax(0.16)) {
        if (makora && makora->IsActive()) {
            makora->Withdraw();
        }
        if (shrine->GetChantPower() < 3.0) {
            shrine->Chant();
            return;
        }
        shrine->WorldCuttingSlashToTarget(this, weakest);
        return;
    }
    if (domain_users.empty() && roll <= 40 && (!this->DomainActive() && !shrine->BurntOut() && this->GetDomainUses() < 5)) {
        this->ActivateDomain();
        return;
    }
    else if (domain_users.size() == 1 && (!this->DomainActive() && !shrine->BurntOut() && this->GetDomainUses() < 5)) {
        this->ActivateDomain();
        return;
    }
    else if ((shrine->BurntOut() || this->GetDomainUses() >= 5) && !domain_users.empty() && !this->CounterDomainActive()) {
        this->ActivateCounterDomain();
        return;
    }
    else if (domain_users.empty() && this->CounterDomainActive()) {
        this->DeactivateCounterDomain();
        return;
    }
    
    if (auto* limitless = dynamic_cast<Limitless*>(weakest->GetTechnique())) {
        if (limitless->CheckInfinity()) {
            this->SetAmplification(true);
            this->Attack(weakest);
            return;
        }
    }
    else {
        this->SetAmplification(false);
    }
    if (this->CEMoreThanMax(0.20)) {
        if (weakest->GetCharacterHealth() < weakest->GetCharacterMaxHealth() * 0.25 && roll <= 15) {
            shrine->UseShrineTechnique(Shrine::ShrineType::Cleave, this, weakest);
            return;
        }
        else if (weakest->CanBeHit()) {
            shrine->UseShrineTechnique(Shrine::ShrineType::Dismantle, this, weakest);
            return;
        }
    }
    this->Attack(weakest); // attack at all costs if havent returned
}

bool Sukuna::CanBeHit() const {
    return true;
}

