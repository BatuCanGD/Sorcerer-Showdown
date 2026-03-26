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
    if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.35 && \
            !(this->GetCharacterCE() > this->GetCharacterMaxCE() * 0.35)) 
    {
        this->BoostRCT();
    }
    else if (this->GetCharacterHealth() <= this->GetCharacterMaxHealth() * 0.75 && \
            !(this->GetCharacterCE() > this->GetCharacterMaxCE() * 0.10)) 
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
        if (target->DomainActive()) {
            domain_users.push_back(target.get());
        }
        if (target->GetCharacterHealth() < weakesthealth) {
            weakesthealth = target->GetCharacterHealth();
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

        if (!makora->IsActivePhysically() && (this->GetCharacterCE() >= this->GetCharacterMaxCE() * 0.235)) {
            makora->Manifest();
        }
        else if (!makora->IsActive() && (this->GetCharacterCE() <= this->GetCharacterMaxCE() * 0.75)) {
            makora->PartiallyManifest();
        }
        else {
            makora->Withdraw();
        }

        if (makora->FullyAdapted() && !shrine->WorldCuttingSlashUnlocked()) {
            this->GetSpecial()->PerformSpecial(this);
            return;
        }
    }

    if (agito && this->GetCharacterHealth() < this->GetCharacterMaxHealth() * 0.35) {
        if (!agito->IsActive()) {
            agito->Manifest();
        }
    }

    int roll = GetRandomNumber(1, 100);

    if (shrine->WorldCuttingSlashUnlocked()) {
        if (shrine->GetChantPower() < 3.0) {
            shrine->Chant();
            return;
        }
        shrine->WorldCuttingSlashToTarget(this, weakest);
        return;
    }
    if (domain_users.size() < 2 && !this->DomainActive() && !this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5) {
        this->ActivateDomain();
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
    if (this->GetCharacterCE() > this->GetCharacterMaxCE() * 0.20) {
        if (weakest->GetCharacterHealth() < weakest->GetCharacterMaxHealth() * 0.25 && roll <= 15) {
            shrine->UseShrineTechnique(Shrine::ShrineType::Cleave, this, weakest);
            return;
        }
        else if (weakest->CanBeHit()) {
            shrine->UseShrineTechnique(Shrine::ShrineType::Dismantle, this, weakest);
            return;
        }
    }
    else {
        this->Attack(weakest);
    }

}

bool Sukuna::CanBeHit() const {
    return true;
}

