#include "Sukuna.h"
#include "DomainList.h"
#include "Shrine.h"
#include "Limitless.h"
#include "MalevolentShrine.h"
#include "ShikigamiList.h"
#include "WorldCuttingSlash.h"
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

std::unique_ptr<Sorcerer> Sukuna::Clone() const {
    return std::make_unique<Sukuna>();
}

std::string Sukuna::GetName() const {
    return "\033[31mSukuna\033[0m";
}
std::string Sukuna::GetSimpleName() const {
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
    bool limitless_found = false;

    for (const auto& target : battlefield) {
        if (target.get() == this) continue;
        double perceived_health = target->GetCharacterHealth() + GetRandomNumber(-100, 100);
        if (target->DomainActive()) {
            domain_users.push_back(target.get());
        }
        bool target_has_limitless = (target->GetTechnique() && target->GetTechnique()->GetTechniqueSimpleName() == "Limitless");

        if (!weakest) {
            weakest = target.get();
            weakesthealth = perceived_health;
            limitless_found = target_has_limitless;
        }
        else if (target_has_limitless && !limitless_found) {
            weakest = target.get();
            weakesthealth = perceived_health;
            limitless_found = true;
        }
        else if (target_has_limitless && limitless_found && perceived_health < weakesthealth) {
            weakest = target.get();
            weakesthealth = perceived_health;
        }
        else if (!limitless_found && perceived_health < weakesthealth) {
            weakest = target.get();
            weakesthealth = perceived_health;
        }
    }

    int tntroll = GetRandomNumber(1, 20);
    if (tntroll <= 11) {
        this->Taunt(weakest);
    }

    Mahoraga* makora = nullptr;
    Agito* agito = nullptr;

    for (const auto& s : shikigami) {
        if (s->GetSimpleName() == "Mahoraga") {
            makora = dynamic_cast<Mahoraga*>(s.get());
        }
        else if (s->GetSimpleName() == "Agito") {
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
        else if (!(agito->IsActive() && this->HPMoreThanMax(0.50)) && this->CEMoreThanMax(0.30)) {
            agito->Manifest();
        }
    }

    int roll = GetRandomNumber(1, 100);

    if (shrine->WorldCuttingSlashUnlocked() && this->CEMoreThanMax(0.16) && roll >= 75) {
        if (makora && makora->IsActive()) {
            makora->Withdraw();
        }
        if (!shrine->FullyChanted()) {
            shrine->Chant();
            return;
        }
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

    if (this->DomainAmplificationActive()) {
        this->SetAmplification(false);
    }
  
    if (roll <= 25 && !shrine->FullyChanted()) {
        shrine->Chant();
        return;
    }
    if (this->CEMoreThanMax(0.20)) {
        if (shrine->FullyChanted()) {
            shrine->UseTheWorldCuttingSlash(this, weakest);
            return;
        }
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

